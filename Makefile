ODIR := bin
GEN_DIR := gen-cpp
GEN_SRC := ProxyRPC.cpp server_constants.cpp server_types.cpp curl.cpp
GEN_OBJ := $(patsubst %.cpp,$(ODIR)/%.o, $(GEN_SRC))
CC := g++

THRIFT_DIR := /usr/local/include
BOOST_DIR := /usr/local/include

INC := -I$(GEN_DIR)/ -I$(THRIFT_DIR) -I$(BOOST_DIR)
LIBS := -L/usr/local/lib/ -lthrift -lcurl

.PHONY: all clean

all: dir client ProxyRPC_server

$(ODIR)/%.o: $(GEN_DIR)/%.cpp
	$(CC) -Wall -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H $(INC) -c $< -o $@

dir:
	mkdir -p $(ODIR)

ProxyRPC_server: $(ODIR)/ProxyRPC_server.o $(GEN_OBJ)
	$(CC) $^ -o $(ODIR)/$@ $(LIBS)

client: $(ODIR)/client.o $(GEN_OBJ)
	$(CC) $^ -o $(ODIR)/$@ $(LIBS)

clean:
	rm -rf $(ODIR)/ *~
