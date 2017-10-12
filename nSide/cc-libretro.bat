@mingw32-make -j4 binary=library target=libretro core=sfc-balanced
@if not exist "out\higan_sfc_balanced_libretro.dll" (pause)