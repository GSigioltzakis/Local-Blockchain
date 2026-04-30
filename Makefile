CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Iinclude -Wno-deprecated-declarations
LDFLAGS = -lssl -lcrypto

TARGET = al

SRCS = src/main.cpp src/block.cpp src/blockchain.cpp src/trans.cpp src/merkle.cpp src/wallet.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# This pattern rule now handles files inside the src/ directory
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) merkle_trees.log wallets.log

run: all
	./$(TARGET)