# Breakout

A C++20 / SFML 3 arcade game with six handcrafted levels, paddle control, ball physics, and brick destruction. The original black background, white arena, and colorful bricks remain.

## Play on Windows without installing C++

Extract the **entire** portable game archive, then double-click `Breakout/breakout.exe`. Keep the `assets` folder next to the executable. No compiler, installer, administrator access, or additional runtime DLLs are needed to play. A graphics driver with OpenGL support is required.

The **Build and test** GitHub Actions workflow produces a `Breakout-Windows-x64` artifact containing the playable folder. Choose a successful run for the branch you want, download the artifact, and extract it.

## Controls

| Action | Controls |
| --- | --- |
| Start a campaign | Enter, Space, or click START |
| Move the paddle | A / D or Left / Right |
| Launch the ball | Space or Enter |
| Pause / resume | P or click PAUSE / RESUME |
| Next level | Space or Enter on the completion screen |
| Replay after winning or game over | Space or Enter |
| Return to menu | Esc during a game |
| Exit | Esc in the menu or close the window |

The ball waits on the paddle before each serve. Move to choose a launch position; the ball's angle after a paddle hit depends on where it lands. Switching to another window automatically pauses the game. Resize the window freely: the arena retains its proportions and mouse buttons use the scaled coordinates.

## Campaign

| Level | Layout |
| --- | --- |
| 1. First Contact | The classic five-row wall |
| 2. Split Decision | Two wings separated by a central channel |
| 3. Diamond | A diamond with an armored center |
| 4. Fortress | Towers, openings, and reinforced defenses |
| 5. Crossfire | Crossing diagonals with armored leading edges |
| 6. Last Stand | A reinforced outer wall and inner core |

The ball gets faster each level. Normal bricks take one hit; white-outlined bricks take two, losing their border when the armor breaks. Destroying a brick earns 100 points; damaging armor earns 25. Clearing a level earns 500 points. Advancing to the next level restores one life, up to five. The campaign starts with three lives and ends with a victory screen after level six.

## Build on Windows with portable tools

Requires 64-bit Windows, PowerShell, Git, and internet for the first build. From a clone of this repository, run:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\build-portable.ps1
```

The script downloads pinned, SHA-256-verified versions of [w64devkit](https://github.com/skeeto/w64devkit/releases/tag/v2.10.0) and [CMake](https://github.com/Kitware/CMake/releases/tag/v3.31.8) into `.tools/`. CMake fetches SFML 3.0.0 and builds the required graphics dependencies. Tools remain local to this folder; the script restores its temporary PATH changes.

It builds, runs the tests, and packages the game in `dist/Breakout/`. Add `-Run` to play after a successful build or `-SkipTests` to explicitly skip verification. Later builds reuse the downloaded tools and dependencies.

## Build with an existing C++ toolchain

Requires a C++20 compiler, Git, and CMake 3.24 or newer. CMake 3.31 is used by the portable build; with CMake 4, older third-party CMake files may require `-DCMAKE_POLICY_VERSION_MINIMUM=3.5`.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
cmake --install build --config Release --component Runtime --prefix dist/Breakout
```

For a Visual Studio generator, the executable and copied assets are in `build/Release/`. For a single-configuration generator they are in `build/`.

On Ubuntu, install graphics development dependencies first:

```sh
sudo apt-get install build-essential cmake git libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libfreetype-dev libgl1-mesa-dev
```

On macOS, install the command-line developer tools and CMake before configuring. The macOS build has not been verified in this change.

## Verification and development

`breakout_tests` covers 18 gameplay scenarios, including collision separation, high-speed hits, armored bricks, life loss, pause/resume, score carry-over, all six level transitions, victory, and restart. It does not need a window.

`breakout_render_test` renders the menu, six levels, gameplay, and overlays to PNGs in `build/screenshots/`. Rendering and startup checks need a working OpenGL context; use `xvfb-run -a ctest --test-dir build --output-on-failure` on a headless Linux machine. The packaged executable also accepts `--check` to check asset loading and render startup without opening a game window.

Add levels in `src/core/Level.cpp`: each string is a row of ten cells, `.` is a gap, `1` a normal brick, and `2` an armored brick. The gameplay model advances in 1/240-second steps and limits catch-up after long stalls.

## License and credits

The game is under the [MIT License](LICENSE). Graphics use SFML. Font rendering uses FreeType: portions of this software are copyright © 2023 The FreeType Project (www.freetype.org). All rights reserved.

The bundled Sansation Regular font by Bernd Montag uses the SIL Open Font License. The existing font family is retained using the Google Fonts distribution with its license included. See [third-party notices](THIRD_PARTY_NOTICES.md) and the `licenses/` directory.
