CC=g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
INC := interface/wav.h
SRC := \
		src/wav.cpp
SLIB := build/wav.a
DLIB := build/wav.so
OBJS := build/wav.o
DEMO := build/demo
DEMO1 := build/demo1
OUTDIR := ./build
TESTDAT := ./test_datas

run:$(DEMO1)
	@echo "************signal channel wav test************"
	LD_LIBRARY_PATH=$(OUTDIR) ./$^ $(TESTDAT)/far_end.wav $(TESTDAT)/echo.wav $(TESTDAT)/mic_with_echo.wav

# run:$(DEMO)
# 	@echo "************signal channel wav test************"
# 	LD_LIBRARY_PATH=$(OUTDIR) ./$^ $(TESTDAT)/sig.wav $(TESTDAT)/echo.wav $(TESTDAT)/combine.wav
# 	# @echo "************double channels wav test************"
# 	# LD_LIBRARY_PATH=$(OUTDIR) ./$^ $(TESTDAT)/a2.wav

build_lib:$(DLIB) $(SLIB)
	@echo build $^


$(OBJS):$(SRC) $(INC)
	$(CC) -c $< -o $@ -I$(dir $(INC))

$(SLIB):$(OBJS)
	ar cqs $(OUTDIR)/libwav.a $^ 

$(DLIB):$(OBJS)
	$(CC) -shared -fPIC -o $(OUTDIR)/libwav.so $^ $(CXXFLAGS) -I$(dir $(INC))

$(DEMO):./demo/demo.cpp $(DLIB)
	$(CC) $< $(CXXFLAGS) -L$(OUTDIR) -lwav -I$(dir $(INC)) -o $@

$(DEMO1):./demo/demo1.cpp $(DLIB)
	$(CC) $< $(CXXFLAGS) -L$(OUTDIR) -lwav -I$(dir $(INC)) -o $@
