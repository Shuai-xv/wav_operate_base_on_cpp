CC=g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

SRC := \
	wav_reader.cpp

test:$(SRC)
	$(CC) $(SRC) -o $(@) $(CXXFLAGS)