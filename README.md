# Domain  Classification

使用url库和cdn库对指定格式dns日志中的域名进行分类

使用makefile进行编译，需要c++11的环境支持

编译的可执行程序是DomainDeal，调用格式：DomainDeal file，file是输入文件

输入文件应使用指定格式，第一行为忽略行，剩下的每行，存在至少15个以tab分隔的短语，其中第3个短语为待分类域名，一个示例的输入文件为test.txt

输出的结果存储在data目录下，其中包含多个文件。_statis.txt是统计结果，_other是无法分类的部分，其他每个文件是分类的类别。
除了统计结果外，文件每一行都是域名的分类结果，未经过去重


