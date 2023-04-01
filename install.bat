mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build . --target hPMML
copy libhPMML.dll* test
cmake --build . --target unit_tests
cd ..
mkdir hpmml_win
copy build\libhPMML.dll* hpmml_win
copy include\hPMML.h hpmml_win
rmdir /Q/S build