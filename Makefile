
INCLUDE= -I ./ -I ./interface
CFLAGS = 
DEFINE := 
CC = gcc
CXX = g++
DIRS :=. ./src
SRC := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
SOBJS := $(patsubst %.c, %.o, $(SRC))

SRCXX := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
SOBJSXX := $(patsubst %.cpp, %.o, $(SRCXX))
# 头文件依赖
DEP := $(patsubst %.o,%.o.d, $(SOBJS))
DEP += $(patsubst %.o,%.o.d, $(SOBJSXX))

TARGET1 = ./build/combine_wav
TARGET2 = ./build/split_wav

demo:$(TARGET1) $(TARGET2)
	@echo "build $^ successfully"

$(TARGET1):$(SOBJS) $(SOBJSXX) ./demo/combine_wav.cpp
	$(CXX) $(INCLUDE) $^ -o $@ -lm

$(TARGET2):$(SOBJS) $(SOBJSXX) ./demo/split_wav.cpp
	$(CXX) $(INCLUDE) $^ -o $@ -lm

# 头文件依赖
-include $(DEP)

$(SOBJS):%.o:%.c
	$(CC) $(DEFINE) $(CFLAGS) -c $< -o $@ -MMD -MF $@.d

$(SOBJSXX):%.o:%.cpp
	$(CXX) $(DEFINE) $(INCLUDE) -c $< -o $@ -MMD -MF $@.d

clean:
	-rm $(SOBJS)
	-rm $(SOBJSXX)
	-rm $(TARGET1)
	-rm $(TARGET2)
	-rm $(DEP)
