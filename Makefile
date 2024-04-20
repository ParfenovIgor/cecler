SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, build/%.o, $(SRCS))

all: prepare $(OBJS) link

build/%.o: src/%.cpp
	g++ $< -c -o $@

link:
	g++ $(OBJS) -o build/cecler

prepare:
	mkdir -p build

clean:
	rm -r build