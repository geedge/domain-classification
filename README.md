# Domain  Classification

使用url库和cdn库对指定格式dns日志中的域名进行分类

使用makefile进行编译，需要c++11的环境支持

编译的可执行程序是DomainDeal，调用格式：DomainDeal file，file是输入文件

输入文件应使用指定格式，第一行为忽略行，剩下的每行，存在至少15个以tab分隔的短语，其中第3个短语为待分类域名，一个示例的输入文件为test.txt

输出的结果存储在data目录下，其中包含多个文件。statis.txt是统计结果，other是无法分类的部分，spcdn是cdn库匹配结果，其他每个文件是文件名对应url库类别的分类结果。
除了统计结果外，文件每一行都是该类别的域名，未经过去重

lib目录中存储两个库，UrlDomainList.dat是url分类库，CdnDomainList.dat是cdn的cname匹配库，都是二进制文件。
程序逻辑是优先匹配url库，其次匹配cdn库。

