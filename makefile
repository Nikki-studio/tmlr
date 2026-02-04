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

BINARY   ?= bin/tmlr
SOURCES  := $(shell find . -name "*.cpp")
OBJECTS  := $(SOURCES:./%.cpp=obj/%.o)
DEPS     := $(OBJECTS:.o=.d)


CXX      := g++
CXXFLAGS := -std=c++23 -g -I./ -O3 -MMD -MP  -Wno-return-type # -Werror
LDFLAGS  := -lncursesw

MAKEFLAGS := --no-builtin-rules --no-builtin-variables
NPROC := $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)


.PHONY: all
all: $(BINARY)

$(BINARY):$(OBJECTS) | bin
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)


obj/%.o: ./%.cpp ./%.hpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include:$(DEPS)
.PHONY: clean run

bin obj:
	mkdir -p $@

run:$(BINARY)
	./$<

clean:
	-rm -r bin obj
	mkdir -p bin obj




