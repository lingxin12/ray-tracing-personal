CXX := g++

CXXFLAGS := -std=c++17

INCLUDE_DIRECTORIES := include src ${shell find src/public -type d -name "*"} src/imgui

INCLUDE_DIRECTORIES_PARAMETER := ${patsubst %,-I%,${INCLUDE_DIRECTORIES}}

SOURCE_FILE := src/main.cc ${shell find src/private -name "*.cc"}

IMGUI_SOURCE_FILE := ${shell find src/imgui -name "*.cpp"}

OBJS := ${patsubst %,output/objs/%, ${notdir ${patsubst %.cc,%.o,${SOURCE_FILE} } } } ${patsubst %,output/objs/%, ${notdir ${patsubst %.cpp,%.o,${IMGUI_SOURCE_FILE}} } }

LIBRARIES_PARAMETER = -lGL `pkg-config --static --libs glfw3` -lpthread

TARGET := output/main

debug :
	@echo ${OBJS}
	@echo
	@echo ${IMGUI_SOURCE_FILE}

run : ${TARGET}
# @echo 555 ./${TARGET}
# @echo
	@./${TARGET}

${TARGET} : ${OBJS}
# @echo 444 ${CXX} -o $@ $^ ${LIBRARIES_PARAMETER}
# @echo
	@${CXX} -o $@ $^ ${LIBRARIES_PARAMETER}

output/objs/main.o : src/main.cc
# @echo 333 ${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}
# @echo
	@${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}

output/objs/%.o : src/private/%.cc
# @echo 222 ${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}
# @echo
	@${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}

output/objs/%.o : src/imgui/%.cpp
# @echo 111 ${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}
# @echo
	@${CXX} ${CXXFLAGS} -c $< -o $@ ${INCLUDE_DIRECTORIES_PARAMETER}

clean :
	@rm -rf output/objs/*

.PHONY: run clean debug