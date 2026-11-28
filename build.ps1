<#
.SYNOPSIS
    Build script for clock_udp (Windows, MinGW).

.DESCRIPTION
    Configures CMake with the MinGW Makefiles generator,
    builds ClockServer and ClockClient.
    CMakeLists.txt automatically runs windeployqt after build,
    so the .exe files are ready to run without a Qt installation.

    The script locates cmake, mingw32-make and Qt6 automatically,
    including the standard Qt Tools paths used by Qt Creator.

.PARAMETER Debug
    Build Debug instead of Release.

.PARAMETER Clean
    Remove the build\ directory before configuring.

.PARAMETER Run
    After build, start the server in background and the client in foreground.

.EXAMPLE
    .\build.ps1
    .\build.ps1 -Clean
    .\build.ps1 -Debug -Run
#>

param(
    [switch]$Debug,
    [switch]$Clean,
    [switch]$Run
)

$ErrorActionPreference = 'Stop'

# --- settings ---
$BuildDir  = "build"
$BuildType = if ($Debug) { "Debug" } else { "Release" }

# ---------------------------------------------------------------------------
# Helper: locate an executable by name in PATH, then in a list of candidate
# paths, then recursively under C:\Qt\Tools as a last resort.
# ---------------------------------------------------------------------------
function Find-Tool {
    param(
        [string]$Name,
        [string[]]$Candidates
    )

    $cmd = Get-Command $Name -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }

    foreach ($c in $Candidates) {
        if (Test-Path $c) { return $c }
    }

    if (Test-Path "C:\Qt\Tools") {
        $found = Get-ChildItem "C:\Qt\Tools" -Recurse -Filter $Name -ErrorAction SilentlyContinue |
                 Select-Object -First 1
        if ($found) { return $found.FullName }
    }

    return $null
}

# --- locate cmake ---
$cmakeExe = Find-Tool -Name "cmake.exe" -Candidates @(
    "C:\Qt\Tools\CMake_64\bin\cmake.exe",
    "C:\Qt\Tools\cmake_64\bin\cmake.exe",
    "C:\Qt\Tools\CMake\bin\cmake.exe",
    "C:\Program Files\CMake\bin\cmake.exe",
    "C:\Program Files (x86)\CMake\bin\cmake.exe"
)

if (-not $cmakeExe) {
    Write-Error "cmake not found. Install CMake or add it to PATH."
    Write-Error "Typical location: C:\Qt\Tools\CMake_64\bin\cmake.exe"
    exit 1
}
Write-Host "Using cmake: $cmakeExe"

# --- locate mingw32-make ---
$mingwMakeExe = Find-Tool -Name "mingw32-make.exe" -Candidates @(
    "C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe",
    "C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe",
    "C:\Qt\Tools\mingw810_64\bin\mingw32-make.exe"
)

if (-not $mingwMakeExe) {
    Write-Error "mingw32-make not found. Install the MinGW toolchain or add it to PATH."
    Write-Error "Typical location: C:\Qt\Tools\mingw*\bin\mingw32-make.exe"
    exit 1
}
Write-Host "Using mingw32-make: $mingwMakeExe"

$mingwBinDir = Split-Path -Parent $mingwMakeExe

# --- locate Qt6 ---
$cmakeExtraArgs = @()
$qtFound = $false
try {
    & $cmakeExe --find-package -DNAME=Qt6 -DCOMPILER_ID=GNU -DLANGUAGE=CXX -DMODE=EXIST 2>$null | Out-Null
    if ($LASTEXITCODE -eq 0) { $qtFound = $true }
} catch { }

$qtDir = $null
if (-not $qtFound) {
    $candidates = @(
        "C:\Qt\6.12.1\mingw_64",
        "C:\Qt\6.8.0\mingw_64",
        "C:\Qt\6.7.0\mingw_64",
        "C:\Qt\6.6.0\mingw_64"
    )
    foreach ($dir in $candidates) {
        if (Test-Path $dir) { $qtDir = $dir; break }
    }

    # Fallback: any Qt 6.x MinGW installation under C:\Qt
    if (-not $qtDir -and (Test-Path "C:\Qt")) {
        $qtRoot = Get-ChildItem "C:\Qt" -Directory -ErrorAction SilentlyContinue |
                  Where-Object { $_.Name -match '^6\.' } |
                  Sort-Object Name -Descending |
                  Select-Object -First 1
        if ($qtRoot) {
            $kit = Get-ChildItem $qtRoot.FullName -Directory -ErrorAction SilentlyContinue |
                   Where-Object { $_.Name -match 'mingw' } |
                   Select-Object -First 1
            if ($kit) { $qtDir = $kit.FullName }
        }
    }

    if ($qtDir) {
        Write-Host "Qt6 not in system PATH, using: $qtDir"
        $cmakeExtraArgs += "-DCMAKE_PREFIX_PATH=$qtDir"
    } else {
        Write-Warning "Qt6 not found in standard paths."
        Write-Warning "Specify it manually: .\build.ps1 -DCMAKE_PREFIX_PATH=C:\Qt\6.x.x\mingw_64"
        exit 1
    }
}

# ---------------------------------------------------------------------------
# Compiler paths.
# Prefer the MinGW toolchain that ships with Qt (under C:\Qt\Tools\mingw*),
# because its version matches the Qt libraries. Fall back to the directory
# that contains mingw32-make.exe.
# ---------------------------------------------------------------------------
$compilerBinDir = $mingwBinDir
$qtMingw = Get-ChildItem "C:\Qt\Tools" -Directory -Filter "mingw*" -ErrorAction SilentlyContinue |
            Sort-Object Name -Descending |
            Select-Object -First 1
if ($qtMingw) {
    $gccCandidate = Join-Path $qtMingw.FullName "bin\gcc.exe"
    if (Test-Path $gccCandidate) {
        $compilerBinDir = Join-Path $qtMingw.FullName "bin"
    }
}

$gccExe = Join-Path $compilerBinDir "gcc.exe"
$gxxExe = Join-Path $compilerBinDir "g++.exe"

if (-not (Test-Path $gccExe) -or -not (Test-Path $gxxExe)) {
    Write-Error "gcc.exe / g++.exe not found in $compilerBinDir"
    exit 1
}
Write-Host "Using gcc: $gccExe"
Write-Host "Using g++: $gxxExe"

# --- clean build ---
if ($Clean -and (Test-Path $BuildDir)) {
    Write-Host ">>> Removing $BuildDir\"
    Remove-Item -Recurse -Force $BuildDir
}

# --- configure ---
Write-Host ">>> CMake configure ($BuildType, MinGW Makefiles)"

$configureArgs = @(
    "-S", ".",
    "-B", $BuildDir,
    "-G", "MinGW Makefiles",
    "-DCMAKE_BUILD_TYPE=$BuildType",
    "-DCMAKE_MAKE_PROGRAM=$mingwMakeExe",
    "-DCMAKE_C_COMPILER=$gccExe",
    "-DCMAKE_CXX_COMPILER=$gxxExe"
) + $cmakeExtraArgs

& $cmakeExe @configureArgs
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# --- build ---
$jobs = [Environment]::ProcessorCount
Write-Host ">>> CMake build (-j $jobs)"
& $cmakeExe --build $BuildDir --config $BuildType -j $jobs
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# --- result ---
$serverCandidates = @(
    Join-Path $BuildDir "server\$BuildType\ClockServer.exe",
    Join-Path $BuildDir "server\ClockServer.exe"
)
$clientCandidates = @(
    Join-Path $BuildDir "client\$BuildType\ClockClient.exe",
    Join-Path $BuildDir "client\ClockClient.exe"
)

$serverBin = $serverCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
$clientBin = $clientCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1

Write-Host ""
Write-Host ">>> Done."
if ($serverBin) { Write-Host "    Server: $serverBin" } else { Write-Warning "    Server binary not found" }
if ($clientBin) { Write-Host "    Client: $clientBin" } else { Write-Warning "    Client binary not found" }

# --- run ---
if ($Run) {
    if (-not $serverBin -or -not $clientBin) {
        Write-Error "Build must succeed before running."
        exit 1
    }
    Write-Host ">>> Starting server in background, client in foreground"
    $serverProc = Start-Process -FilePath $serverBin -PassThru
    Start-Sleep -Seconds 1
    try {
        & $clientBin
    } finally {
        if ($serverProc -and -not $serverProc.HasExited) {
            Stop-Process -Id $serverProc.Id -Force
        }
    }
}