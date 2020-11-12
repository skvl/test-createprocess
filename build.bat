meson setup build/x86/release --buildtype=release --cross-file x86-windows.ini
meson compile -C build/x86/release

meson setup build/x64/release --buildtype=release --cross-file x64-windows.ini
meson compile -C build/x64/release
