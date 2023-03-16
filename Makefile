CXX := g++

CXXFLAGS := -std=c++17

INCLUDE_DIRECTORIES := include src ${shell find src/public -type d -name "*"}

INCLUDE_DIRECTORIES_PARAMETER := ${patsubst %,-I%,${INCLUDE_DIRECTORIES}}

SOURCE_FILE := src/main.cc ${shell find src/private -name "*.cc"}

SRC := src

OBJ := output/objs

OBJS := ${patsubst %,${OBJ}/%, ${notdir ${patsubst %.cc,%.o,${SOURCE_FILE} } } }

LIBRARIES := pthread

LIBRARIES_PARAMETER = ${patsubst %,-l%,${LIBRARIES}}

TARGET := output/main

run : ${TARGET}
	@./${TARGET}

${TARGET} : ${OBJS}
	@${CXX} -o $@ $^ ${LIBRARIES_PARAMETER}

${OBJ}/main.o : ${SRC}/main.cc
	@${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}

${OBJ}/%.o : ${SRC}/private/%.cc
	@${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}

clean :
	@rm -rf output/objs/*

.PHONY: run clean