#!/bin/sh
rm -rf build;
mkdir build && cd build && cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release && make