PHEET_DIR = ../../pheet

bake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE="Debug" ..; make -j6
	
check: bake
	cd build; ctest --output-on-failure

bench:
	cd $(PHEET_DIR) && make clean test && bin/pheet_test

clean:
	rm -rf build
