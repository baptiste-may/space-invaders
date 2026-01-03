#!/bin/bash

source /etc/os-release
case "$ID" in
"debian" | "ubuntu")
  sudo apt-get install -y build-essential git make doxygen \
    pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
    libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
    libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
    libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
    libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev \
    libfreetype6-dev libharfbuzz-dev libncurses5-dev libncursesw5-dev valgrind
  ;;
"fedora")
  sudo dnf install -y gcc git-core make cmake doxygen \
    alsa-lib-devel fribidi-devel pulseaudio-libs-devel pipewire-devel \
    libX11-devel libXext-devel libXrandr-devel libXcursor-devel libXfixes-devel \
    libXi-devel libXScrnSaver-devel libXtst-devel dbus-devel ibus-devel \
    systemd-devel mesa-libGL-devel libxkbcommon-devel mesa-libGLES-devel \
    mesa-libEGL-devel vulkan-devel wayland-devel wayland-protocols-devel \
    libdrm-devel mesa-libgbm-devel libusb1-devel libdecor-devel \
    pipewire-jack-audio-connection-kit-devel libthai-devel freetype-devel \
    harfbuzz-devel ncurses-devel valgrind
  sudo dnf install -y liburing-devel
  sudo dnf install -y zlib-ng-compat-static
  ;;
esac

INIT_PATH="$PWD"

# Check if submodules are initialized
if [ ! -f "3rdParty/SDL/CMakeLists.txt" ] || [ ! -f "3rdParty/SDL_image/CMakeLists.txt" ] || [ ! -f "3rdParty/SDL_ttf/CMakeLists.txt" ]; then
  echo "Submodules not found or incomplete. Initializing..."
  git submodule update --init --recursive
fi

if [ ! -d 3rdParty/SDL/build ]; then
  cd 3rdParty/SDL
  mkdir build
  cd build
  cmake ..
  make -j
fi
cd "$INIT_PATH"

if [ ! -d 3rdParty/SDL_image/build ]; then
  cd 3rdParty/SDL_image
  mkdir build
  cd build
  SDL3_DIR="$INIT_PATH/3rdParty/SDL/build" cmake ..
  make -j
fi
cd "$INIT_PATH"

if [ ! -d 3rdParty/SDL_ttf/build ]; then
  cd 3rdParty/SDL_ttf
  mkdir build
  cd build
  SDL3_DIR="$INIT_PATH/3rdParty/SDL/build" cmake ..
  make -j
fi
cd "$INIT_PATH"

export LD_LIBRARY_PATH=3rdParty/SDL/build:3rdParty/SDL_image/build:3rdParty/SDL_ttf/build:$LD_LIBRARY_PATH
