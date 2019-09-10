PROJECT=server
CC=gcc
CFLAGS=-W -g

CXX=g++
CXXFLAGS=-W -std=c++11 -g
PKG_FLAG=`pkg-config --libs --cflags glib-2.0`
LINK=-Wl,-rpath,$(JEMALLOC_DIR)/lib

LIB_DIR:=lib
SRC_DIR:=./common
INC_DIR:=include
THIRD_PART_DIR:=3rd
MSGPACK_DIR:=$(THIRD_PART_DIR)/msgpack
TINYXML_DIR:=$(THIRD_PART_DIR)/tinyxml2

INC_FILES:=-I$(INC_DIR) \
	-I$(MSGPACK_DIR)/include/\
	-I$(TINYXML_DIR)/include/\
	-I/usr/include/python3.5

C_SRC_FILES:=$(foreach v, $(SRC_DIR), $(wildcard $(v)/*.c))
CXX_SRC_FILES:=$(foreach v, $(SRC_DIR), $(wildcard $(v)/*.cpp))

C_OBJ_FILES:=$(subst .c,.o,$(C_SRC_FILES))
CXX_OBJ_FILES:=$(subst .cpp,.o,$(CXX_SRC_FILES))


LIBS=-L$(MSGPACK_DIR)/lib \
	-L$(TINYXML_DIR)/lib

STATIC_LIBS=-lpython3.5m \
	-ltinyxml2 \

all:$(C_OBJ_FILES) $(CXX_OBJ_FILES)
	#$(CC) $(CFLAGS) $(C_OBJ_FILES) $(CXX_OBJ_FILES) -o $(PROJECT) $(LIBS) $(STATIC_LIBS) $(PKG_FLAG) $(LINK)
	$(CXX) $(CFLAGS) $(CXX_OBJ_FILES) $(CXX_OBJ_FILES) -o $(PROJECT)

$(SRC_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -c -o $@ $(INC_FILES) $(LIBS) $(STATIC_LIBS) $(PKG_FLAG)

$(SRC_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@ $(INC_FILES) $(LIBS) $(STATIC_LIBS)

.PHONY:clean rpc
clean:
	-rm $(PROJECT)
	-rm -rf $(SRC_DIR)/*.o
rpc:
	@python tools/parse_rpc.py
