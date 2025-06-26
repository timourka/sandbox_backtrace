cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j1
file sandbox
readelf -S sandbox | grep debug
addr2line -e ./sandbox 0x55af4bb3d78f
./sandbox &
pid=$!
cat /proc/$pid/maps | grep sandbox
