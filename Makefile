all: stegim

#----------------------------------------------------------
ifeq ($(DEBUG),ON)
DEBUG_FLAG=-DNDEBUG=2 -DDEBUG_COLOR_ENABLE
else
DEBUG_FLAG=
endif

#----------------------------------------------------------
GCC_FLAGS=-Wall -Wextra -O3

#----------------------------------------------------------
SOURCES_PATH=src
HEADERS_PATH=headers

#----------------------------------------------------------
BIN_PATH=bin
BIN_NAME=stegim

#----------------------------------------------------------
ALL_CPP_FILES:=$(shell find ${SOURCES_PATH} -name *.cpp)
ALL_HPP_FILES:=$(shell find ${HEADERS_PATH} -name *.hpp)

#----------------------------------------------------------
stegim:	Makefile ${BIN_PATH} ${ALL_CPP_FILES} ${ALL_HPP_FILES}
	g++ ${ALL_CPP_FILES} -I${HEADERS_PATH} ${GCC_FLAGS} -o ${BIN_PATH}/${BIN_NAME} ${DEBUG_FLAG}

#----------------------------------------------------------
${BIN_PATH}:
		mkdir ${BIN_PATH}

#----------------------------------------------------------
print_info:
	@echo "GCC_FLAGS: ${GCC_FLAGS}"
	@echo "SOURCES_PATH: ${SOURCES_PATH}"
	@echo "HEADERS_PATH: ${HEADERS_PATH}"
	@echo "ALL_CPP_FILES: ${ALL_CPP_FILES}"
	@echo "ALL_HPP_FILES: ${ALL_HPP_FILES}" 
