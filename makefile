CXX=g++
CXXFLAGS=-std=c++17 -Wall -I include/
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system -pthread
SRC_DIR=./src
OBJ_DIR=./obj
TEST_DIR=./tests
TEST_OUT_DIR=./tests/bin
BUILD_DIR=./build

BRACKET_STYLE=java

OUTPUT=NoiseDisplay

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)

OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
TEST_OUT := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_DIR)/bin/%.test,$(TEST_FILES))

.DEFAULT: all
.PHONY: all default client test preprocess prettify objprepender clientprepender

all: clientprepender client

client: ${OBJ_FILES}
	${CXX} ${CXXFLAGS} $^ -o ${BUILD_DIR}/${OUTPUT} ${LDLIBS}

test: ${TEST_OUT}	

preprocess: CXXFLAGS+=-E
preprocess: default

debug: CXXFLAGS+=-g 
debug: clean all 

${TEST_OUT_DIR}/%.test: ${OBJ_FILES} ${TEST_DIR}/%.cpp 
	$(CXX) ${CXXFLAGS} $^ -o $@ ${LDLIBS}
	$@

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp 
	$(CXX) ${CXXFLAGS} -c -o $@ $< ${LDLIBS}

clientprepender: 
	@echo ==============================================
	@echo ==============MAKING=CLIENT===================
	@echo ==============================================

prettify:
	-find -type f \( -iname \*.h -o -iname \*.hpp -o -iname \*.cpp \) -exec astyle -n --style=${BRACKET_STYLE} {} \;

clean:
	-rm -rf ${BUILD_DIR}/*
	-rm -rf ${OBJ_DIR}/*.o
	-rm -rf ${OBJ_DIR}/*/*.o
	-rm -rf ${TEST_DIR}/*.test
