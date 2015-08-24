rem For use with Windows
@echo off
mkdir "obj"

rem Accuracy profile
mkdir "obj\accuracy"
move "obj\accuracy\sfc-*.o" "obj"
move "obj\accuracy\ethos-ethos.o" "obj"
move "obj\accuracy\ethos-settings.o" "obj"
mingw32-make -j4 target=nSide-e profile=accuracy console=true
move "obj\sfc-*.o" "obj\accuracy"
move "obj\ethos-ethos.o" "obj\accuracy"
move "obj\ethos-settings.o" "obj\accuracy"
if not exist "out\nSide-e.exe" (pause)

@echo on