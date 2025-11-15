# 核心变量：可执行文件与源文件同级
SRC := wienerAttack.cpp          # 你的C++源文件名（请根据实际文件名修改）
TARGET := $(basename $(SRC))# 可执行文件名（与源文件同名，如rsa_ntl.cpp→rsa_ntl）
CXX := g++                  # 编译器
CXXFLAGS := -std=c++11 -O2  # 编译选项：C++11标准+优化等级O2（保证计时和模板正常工作）
LDFLAGS := -lntl            # 链接NTL库（必选，否则无法使用大整数/数论函数）

# 默认目标：直接编译生成同级可执行文件
all: $(TARGET)

# 编译+链接逻辑：源文件→同级可执行文件
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
	@echo "✅ 编译完成！可执行文件：./$(TARGET)（与源文件同级）"

# 清理目标：删除生成的可执行文件
clean:
	rm -f $(TARGET)  # -f:文件不存在时不报错
	@echo "🗑️  清理完成！已删除可执行文件./$(TARGET)"

# 伪目标声明：避免与同名文件冲突
.PHONY: all clean