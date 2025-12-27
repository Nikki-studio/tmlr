## makefile
#  Copyright (C) 2024  Joseph Wangai Mwaniki joewamwaniki@gmail.com
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

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


