
run: build
	./build/main

build: clean
	cmake -S . -B build
	cmake --build build 

clean:
	rm -rf build/*