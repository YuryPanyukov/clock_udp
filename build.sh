#!/usr/bin/env bash
# Сборка проекта clock_udp (Linux/macOS)
#
# Использование:
#   ./build.sh                # Release-сборка
#   ./build.sh --debug        # Debug-сборка
#   ./build.sh --clean        # удалить build/ и собрать заново
#   ./build.sh --run          # собрать и запустить сервер+клиент
#   ./build.sh --help

set -euo pipefail

# --- настройки ---
BUILD_DIR="build"
BUILD_TYPE="Release"
DO_CLEAN=0
DO_RUN=0

# --- разбор флагов ---
for arg in "$@"; do
    case "$arg" in
        --debug) BUILD_TYPE="Debug" ;;
        --release) BUILD_TYPE="Release" ;;
        --clean) DO_CLEAN=1 ;;
        --run) DO_RUN=1 ;;
        --help|-h)
            sed -n '2,12p' "$0" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        *)
            echo "Неизвестный аргумент: $arg" >&2
            echo "См. ./build.sh --help" >&2
            exit 1
            ;;
    esac
done

# --- проверка окружения ---
command -v cmake >/dev/null 2>&1 || {
    echo "Ошибка: cmake не найден. Установите: sudo apt install cmake" >&2
    exit 1
}
command -v make >/dev/null 2>&1 || command -v ninja >/dev/null 2>&1 || {
    echo "Ошибка: ни make, ни ninja не найдены." >&2
    exit 1
}

# --- Qt6: если не системный, попробовать найти ---
CMAKE_EXTRA_ARGS=()
if ! cmake --find-package -DNAME=Qt6 -DCOMPILER_ID=GNU -DLANGUAGE=CXX -DMODE=EXIST >/dev/null 2>&1; then
    for qtdir in \
        "$HOME/Qt/6.8.0/gcc_64" \
        "$HOME/Qt/6.7.0/gcc_64" \
        "/opt/Qt/6.8.0/gcc_64" \
        "/opt/Qt/6.7.0/gcc_64" \
        "/usr/lib/x86_64-linux-gnu/cmake/Qt6"; do
        if [ -d "$qtdir" ]; then
            echo "Qt6 не в системном PATH, использую: $qtdir"
            CMAKE_EXTRA_ARGS+=("-DCMAKE_PREFIX_PATH=$qtdir")
            break
        fi
    done
fi

# --- чистая сборка ---
if [ "$DO_CLEAN" -eq 1 ]; then
    echo ">>> Удаляю $BUILD_DIR/"
    rm -rf "$BUILD_DIR"
fi

# --- конфигурация ---
echo ">>> CMake configure ($BUILD_TYPE)"
cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    "${CMAKE_EXTRA_ARGS[@]}"

# --- сборка ---
JOBS="$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)"
echo ">>> CMake build (-j $JOBS)"
cmake --build "$BUILD_DIR" -j "$JOBS"

# --- результат ---
SERVER_BIN="$BUILD_DIR/server/ClockServer"
CLIENT_BIN="$BUILD_DIR/client/ClockClient"

echo
echo ">>> Готово."
echo "    Сервер: $SERVER_BIN"
echo "    Клиент: $CLIENT_BIN"

# --- запуск ---
if [ "$DO_RUN" -eq 1 ]; then
    echo ">>> Запускаю сервер в фоне, клиент на переднем плане"
    "$SERVER_BIN" &
    SERVER_PID=$!
    trap 'kill $SERVER_PID 2>/dev/null || true' EXIT
    sleep 1
    "$CLIENT_BIN"
fi