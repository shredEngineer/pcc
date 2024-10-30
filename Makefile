# PCC - Paul's Code Collection
# A powerful cross-platform script interpreter that is extensible.
# Copyright (C) 2016-2017 by Paul Wilhelm <anfrage@paulwilhelm.de>
#
# PCC is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PCC is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.


# Project version <Maj.Min.Rev> (ensure to also change this in Doxyfile)
VERSION=0.1.0


# Target binary
TARGET=bin/pcc


# Arguments passed to binary in 'test' target
TEST_ARGS=tutorial/test.pcc


# Source- & object folders
SOURCES_DIR=src/
OBJECTS_DIR=obj/


# Linker flags
LDFLAGS=


# FIND all source files in source folder (recursive)
SOURCES_TEMP=$(shell find $(SOURCES_DIR) -type f -name \*.cpp)
SOURCES=$(SOURCES_TEMP:$(SOURCES_DIR)%=%)

# Include all source subdirectories
SOURCE_DIRS=$(shell find $(SOURCES_DIR) -type d)
SOURCE_INCL=-I$(subst $(subst ,, ), -I,$(SOURCE_DIRS))


# Move all object files to object folder
OBJECTS_TEMP=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst %,$(OBJECTS_DIR)%,$(OBJECTS_TEMP))


# Doxygen configuration
DOXYGEN_DIR=doc/
DOXYGEN_CONF=Doxyfile


# Software tools used
CC=g++
RM=rm
DOXYGEN=doxygen
CLEAR=clear


# Compiler flags
CFLAGS=-std=gnu++11 -g -O3 -Wall -Wextra -pedantic -DVERSION=\"$(VERSION)\" $(SOURCE_INCL)


# Show configuration & available Makefile targets
help:
	@echo "PCC project Makefile"
	@echo "Copyright (C) 2016-2017 by Paul Wilhelm <anfrage@paulwilhelm.de>"
	@echo
	@echo "Configuration:"
	@echo "    Project version: $(VERSION)"
	@echo "    Target binary:   $(TARGET)"
	@echo "    Test arguments:  $(TEST_ARGS)"
	@echo
	@echo "    Doxygen config:  $(DOXYGEN_DIR)$(DOXYGEN_CONF)"
	@echo "    Objects dir:     $(OBJECTS_DIR)"
	@echo "    Sources dir:     $(SOURCES_DIR)"
	@echo
	@echo "    Source files:    $(SOURCES)"
	@echo
	@echo "Available Makefile targets:"
	@echo "    help . . . . . . Show configuration & available Makefile targets"
	@echo "    all  . . . . . . Execute the following targets: clean build test"
	@echo "    doc  . . . . . . Generate full project documentation (Doxygen)"
	@echo "    clean  . . . . . Remove all object files and target binary"
	@echo "    build  . . . . . Generate all object files and target binary"
	@echo "    test . . . . . . Run target binary with test arguments"
	@echo "    user . . . . . . Run target binary in interactive mode"


# Execute the following targets: clean build test
all: clean build test


# Generate full project documentation (Doxygen)
.PHONY: doc

doc:
	@echo "[Doxygen] $(DOXYGEN_DIR)"
	@$(DOXYGEN) $(DOXYGEN_DIR)$(DOXYGEN_CONF)


# Remove all object files and target binary
clean:
	@echo "[ Clean ] $(OBJECTS_DIR)"
	@$(RM) -f $(OBJECTS)
	@echo "[ Clean ] $(TARGET)"
	@$(RM) -f $(TARGET)


# Generate all object files and target binary
build: $(OBJECTS)
	@echo "[ Build ] $(TARGET)"
	@$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Rule for compiling all both *.cpp and corresponding *.hpp files
# NOTE: A corresponding .hpp file must exist for every .cpp file
$(OBJECTS_DIR)%.o : $(SOURCES_DIR)%.cpp $(SOURCES_DIR)%.hpp
	@echo "[Compile] $<"
	@$(CC) -c -o $@ $< $(CFLAGS)


# Run target binary with test arguments
test:
	@echo "[  Run  ] $(TARGET) $(TEST_ARGS)"
	@echo
	$(CLEAR)
	./$(TARGET) $(TEST_ARGS)


# Run target binary in interactive mode
user:
	@echo "[  Run  ] $(TARGET)"
	@echo
	$(CLEAR)
	./$(TARGET)
