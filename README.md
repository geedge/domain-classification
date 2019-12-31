# Domain  Classification

使用url库和cdn库对指定格式dns日志中的域名进行分类

使用makefile进行编译，需要c++11的环境支持

编译的可执行程序是DomainDeal，调用格式：DomainDeal file，file是输入文件

输入文件应使用指定格式，第一行为忽略行，剩下的每行，存在至少15个以tab分隔的短语，其中第3个短语为待分类域名
