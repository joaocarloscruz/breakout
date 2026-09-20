#include "ui/include/MainMenu.h"
#include "ui/include/Layout.h"
#include "core/include/GameManager.h"
#include <filesystem>
#include <iostream>
#include <string_view>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace {
std::filesystem::path executableDirectory(const char* argument) {
#ifdef _WIN32
    wchar_t path[32768];
    const auto length = GetModuleFileNameW(nullptr, path, 32768);
    if (length > 0 && length < 32768) return std::filesystem::path(path).parent_path();
#elif defined(__linux__)
    std::error_code error;
    const auto path = std::filesystem::read_symlink("/proc/self/exe", error);
    if (!error) return path.parent_path();
#elif defined(__APPLE__)
    char path[4096];
    std::uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        return std::filesystem::weakly_canonical(path).parent_path();
#endif
    return std::filesystem::absolute(argument).parent_path();
}
}

int main(int argc, char** argv) {
    const auto fontPath = executableDirectory(argc > 0 ? argv[0] : "breakout") / "assets/font/sansation.ttf";
    sf::Font font;
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Cannot load " << fontPath << ". Keep the assets folder beside breakout.\n";
#ifdef _WIN32
        MessageBoxW(nullptr, L"Cannot load assets/font/sansation.ttf. Extract the complete game folder before launching.",
                    L"Breakout - missing font", MB_OK | MB_ICONERROR);
#endif
        return 1;
    }
    // Also exercises asset lookup when run from an unrelated working directory.
    if (argc == 2 && std::string_view(argv[1]) == "--check") {
        // Windows CI's software OpenGL driver only supports 1024px textures.
        sf::RenderTexture target({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
        target.setView(sf::View(sf::FloatRect({0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT})));
        MainMenu menu(font);
        target.clear();
        menu.draw(target);
        target.display();
        GameManager game(font);
        target.clear();
        game.draw(target);
        target.display();
        return 0;
    }
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Breakout");
    window.setFramerateLimit(120);
    window.setKeyRepeatEnabled(false);
    updateView(window);
    MainMenu menu(font);
    menu.start(window);
    return 0;
}
