run: build
	./build/async_app

build: cache
	cmake --build build

cache:
	cmake . -B build

clean:
	rm -rf build
