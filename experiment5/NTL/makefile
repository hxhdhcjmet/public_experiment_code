CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
# 链接选项
LDFLAGS = -lntl -lgmp

TARGET = baby_step_giant_step

SRC = experiment52.cpp

all:$(TARGET)

# $符号表示取变量的值，当变量名多于一个字符时，使用"( )"
# $符的其他用法

# $^ 表示所有的依赖文件

# $@ 表示生成的目标文件

# $< 代表第一个依赖文件
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 清理目标
clean:
	rm -f $(TARGET)

.PHONY:all clean


