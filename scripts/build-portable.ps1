param([switch]$Run, [switch]$SkipTests)
$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot
$toolRoot = Join-Path $repo '.tools'
$buildRoot = Join-Path $repo 'build-portable'
New-Item -ItemType Directory -Force -Path $toolRoot | Out-Null

function Get-VerifiedDownload($url, $destination, $sha256) {
    if (!(Test-Path -LiteralPath $destination)) {
        Write-Host "Downloading $(Split-Path -Leaf $destination)..."
        Invoke-WebRequest -UseBasicParsing -Uri $url -OutFile $destination
    }
    if ((Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash -ne $sha256) {
        throw "Checksum mismatch: $destination. Remove that file and retry."
    }
}

$compilerArchive = Join-Path $toolRoot 'w64devkit-x64-2.10.0.7z.exe'
$compiler = Join-Path $toolRoot 'w64devkit/bin/g++.exe'
if (!(Test-Path -LiteralPath $compiler)) {
    Get-VerifiedDownload 'https://github.com/skeeto/w64devkit/releases/download/v2.10.0/w64devkit-x64-2.10.0.7z.exe' $compilerArchive '18d0a4c71a166f8401ab6305781bec5882b40b5e06ba9807c61cb5f3b3c6325e'
    $extract = Start-Process -FilePath $compilerArchive -ArgumentList '-y', ('-o"' + $toolRoot + '"') -WindowStyle Hidden -Wait -PassThru
    if ($extract.ExitCode -ne 0) { throw 'Compiler extraction failed.' }
}
$cmakeRoot = Join-Path $toolRoot 'cmake-3.31.8-windows-x86_64'
$cmake = Join-Path $cmakeRoot 'bin/cmake.exe'
if (!(Test-Path -LiteralPath $cmake)) {
    $archive = Join-Path $toolRoot 'cmake-3.31.8-windows-x86_64.zip'
    Get-VerifiedDownload 'https://github.com/Kitware/CMake/releases/download/v3.31.8/cmake-3.31.8-windows-x86_64.zip' $archive '81aa9964dbabd71fe02e7ec50472fd3ad56138c49944515ece9001efbff8d719'
    Expand-Archive -LiteralPath $archive -DestinationPath $toolRoot -Force
}

# Only this process sees these tools; no permanent PATH changes.
$previousPath = $env:PATH
try {
    $env:PATH = "$(Join-Path $toolRoot 'w64devkit/bin');$(Join-Path $cmakeRoot 'bin');$previousPath"
    $testing = if ($SkipTests) { 'OFF' } else { 'ON' }
    & $cmake -S $repo -B $buildRoot -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DSFML_USE_STATIC_STD_LIBS=ON "-DBUILD_TESTING=$testing"
    if ($LASTEXITCODE -ne 0) { throw 'CMake configure failed.' }
    & $cmake --build $buildRoot --parallel 4
    if ($LASTEXITCODE -ne 0) { throw 'Build failed.' }
    if (!$SkipTests) {
        & (Join-Path $cmakeRoot 'bin/ctest.exe') --test-dir $buildRoot --output-on-failure
        if ($LASTEXITCODE -ne 0) { throw 'Gameplay tests failed.' }
    }
    & $cmake --install $buildRoot --component Runtime --prefix (Join-Path $repo 'dist/Breakout')
    if ($LASTEXITCODE -ne 0) { throw 'Packaging failed.' }
    Write-Host "Ready: $(Join-Path $repo 'dist/Breakout/breakout.exe')"
    if ($Run) { & (Join-Path $repo 'dist/Breakout/breakout.exe') }
} finally {
    $env:PATH = $previousPath
}
