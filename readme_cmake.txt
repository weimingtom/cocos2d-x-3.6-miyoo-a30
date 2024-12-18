mkdir linux
cd linux
cmake -G "Unix Makefiles" -DBUILD_CPP_TESTS=ON -DBUILD_LUA_LIBS=OFF ..

make -j8
(make VERBOSE=1 > a.txt 2>&1)

