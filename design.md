# 设计

## 宏观类设计

- 缓冲区类Buffer
	- **不使用**配对缓冲区+双指针策略
	- 链接流到文件
	- 实现字符指针的向前与回退
	- 实现行计数、字符计数
	- 提供函数nextChar，返回字符
	- 提供函数nextNbChar，返回下一个非空字符
	- 提供函数retract，使向前指针回退
	- 提供函数setError，提供错误定位
- 自动机类Automata
	- 使用Buffer的nextChar和nextNbChar函数获取字符
	- 使用DFA判断Buffer中的token类型
	- 提供函数nextNotation，返回下一个记号
- 计数器Counter
	- 接收Automata返回的记号
	- 实现关键字计数、符号表生成