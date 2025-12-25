binary   ?= tmlr
SRC_DIRS := ./ src lib utils
sources  := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
objects  := $(patsubst %cpp , obj/%.o , $(sources))
flags    := -std=c++23 -g -I./ -O3 -MMD -MP  -Wno-return-type # -Werror

all: $(binary)
bin/$(binary): $(binary) | bin

$(binary):$(objects) | bin
	g++ $(flags) $^ -o bin/$@

obj/%.o: ./%.cpp ./%.hpp
	g++ $(flags) -c $< -o $@


bin obj:
	mkdir -p $@

run:$(binary)
	./bin/$<

clean:
	-rm -r bin obj
	mkdir -p bin obj


.PHONY:all clean run


