PROJECT=engine

CXX=g++
CXXFLAGS=-W -std=c++11 -g
LINK=-Wl,-rpath,$(JEMALLOC_DIR)/lib

LIB_DIR:=lib
INC_DIR:=include
THIRD_PART_DIR:=3rd
MSGPACK_DIR:=$(THIRD_PART_DIR)/msgpack
TINYXML_DIR:=$(THIRD_PART_DIR)/tinyxml2

INC_FILES:=-I$(INC_DIR) \
	-I$(MSGPACK_DIR)/include/ \
	-I$(TINYXML_DIR)/include/ \
	-I/usr/include/python3.5 \
	-I/usr/local/include/mongocxx/v_noabi \
	-I/usr/local/include/bsoncxx/v_noabi

CXX_SRC_FILES:=$(wildcard ./common/*.cpp) \
	$(wildcard ./centerd/*.cpp) \
	$(wildcard ./gamed/*.cpp) \
	$(wildcard ./gated/*.cpp) \
	$(wildcard ./dbd/*.cpp) \

CXX_OBJ_FILES:=$(subst .cpp,.o,$(CXX_SRC_FILES))

LIBS=-L$(MSGPACK_DIR)/lib \
	-L$(TINYXML_DIR)/lib \
	-L/usr/local/lib

STATIC_LIBS=-lpython3.5m \
	-ltinyxml2 \
	-lmongocxx \
	-lbsoncxx

all:$(CXX_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(CXX_OBJ_FILES) -o $(PROJECT) $(LIBS) $(STATIC_LIBS)

$(CXX_OBJ_FILES):%.o:%.cpp
	$(CXX) $(CXXFLAGS) $< -c $@ $(INC_FILES)

.PHONY:clean rpc stop
clean:
	-rm $(PROJECT)
	-rm -rf $(CXX_OBJ_FILES)
rpc:
	cd proto/def && protoc --proto_path=./ * --python_out=../output
stop:
	pkill engine
