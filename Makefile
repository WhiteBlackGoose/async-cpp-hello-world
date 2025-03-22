run: build
	./build/async_app

build: cache
	cmake --build build

cache: clean
	cmake . -B build

clean:
	rm -rf build
