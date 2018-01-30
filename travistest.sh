#!/bin/sh
util/premake5 gmake
make clean
util/premake5 gmake
make -j &
make config=test -j &
wait
bin/dbg/test_vaddr &
bin/test/test_vaddr &
wait