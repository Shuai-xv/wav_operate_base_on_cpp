CC=g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
INC := wav.h
SRC := \
	wav.cpp

SLIB := wav.a
DLIB := wav.so
OBJS := wav.o
DEMO := demo


run:$(DEMO)
	@echo "************signal channel wav test************"
	LD_LIBRARY_PATH=. ./$^ sig.wav echo.wav combine.wav
	# @echo "************double channels wav test************"
	# LD_LIBRARY_PATH=. ./$^ ./a2.wav

$(OBJS):$(SRC) $(INC)
	$(CC) -c $< -o $@

$(SLIB):$(OBJS)
	ar cqs libwav.a $^

$(DLIB):$(OBJS)
	$(CC) -shared -fPIC -o libwav.so $^ $(CXXFLAGS)

$(DEMO):demo.cpp $(DLIB)
	$(CC) $< $(CXXFLAGS) -L. -lwav -o $@