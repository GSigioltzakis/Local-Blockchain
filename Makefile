CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Iinclude -Wno-deprecated-declarations
LDFLAGS = -lssl -lcrypto

TARGET = al

SRCS = src/main.cpp src/block.cpp src/blockchain.cpp src/trans.cpp src/merkle.cpp src/wallet.cpp

OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o $(TARGET) merkle_trees.log wallets.log 

run: all
	./$(TARGET)