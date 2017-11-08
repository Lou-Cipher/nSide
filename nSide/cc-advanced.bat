@echo off
rem For use with Windows

echo "libretro"  fork of higan's libretro core altered to use the balanced profile
echo "star-rod"  fork of laevateinn, the debugger included with bsnes v086
echo "prealpha"  enable support for unfinished emulators: Atari 2600, SG-1000
echo "shimai"    raster UI inspired by the NES Classic Edition
echo "console"   show console window during emulation
set /p target=Target:

if /i "%target%" equ "libretro" (
  mingw32-make -j4 binary=library target=libretro core=sfc-balanced
  if not exist "out\higan_sfc_balanced_libretro.dll" (pause)
)
if /i "%target%" equ "star-rod" (
  mingw32-make -j4 target=star-rod
  if not exist "out\star-rod.exe" (pause)
)
if /i "%target%" equ "prealpha" (
  rem Accuracy profile
  mingw32-make -j4 console=true prealpha=true
  if not exist "out\nSide.exe" (pause)

  rem Balanced profile
  mingw32-make -j4 console=true prealpha=true profile=balanced name=nSide-balanced
  if not exist "out\nSide-balanced.exe" (pause)
)
if /i "%target%" equ "shimai" (
  mingw32-make -j4 target=shimai
  if not exist "out\shimai.exe" (pause)
)
if /i "%target%" equ "console" (
  rem Accuracy profile
  mingw32-make -j4 console=true prealpha=true
  if not exist "out\nSide.exe" (pause)

  rem Balanced profile
  mingw32-make -j4 console=true prealpha=true profile=balanced name=nSide-balanced
  if not exist "out\nSide-balanced.exe" (pause)
)

@echo on