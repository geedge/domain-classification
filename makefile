#编译器版本
CC = gcc
CXX = g++ -std=c++11
#附加头文件目录
INCS_DIR =
#附加包含库目录
LIBS_DIR =
#附加包含库文件
DEF_LIBS =
STA_LIBS =
DYN_LIBS =
#预定义宏
DEFINE = -D MAKEFILE_ENVIRONMENT
#编译选项
CFLAGS = -g -Wall -O3

#目标生成文件名
TARGETS = DomainDeal
#源文件
C_SRCS = $(wildcard *.c */*.c)
CPP_SRCS = $(wildcard *.cpp */*.cpp)
#中间文件
C_OBJS = $(patsubst %.c,%.o,$(C_SRCS))
CPP_OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS))
#依赖头文件
HEADS = $(wildcard *.h */*.h *.hpp */*.hpp)


#生成目标
$(TARGETS): $(C_OBJS) $(CPP_OBJS)
	$(CXX) -o $@ $(CFLAGS) $(LIBS_DIR) $(C_OBJS) $(CPP_OBJS) \
	$(DEF_LIBS) -Wl,-Bstatic $(STA_LIBS) -Wl,-Bdynamic $(DYN_LIBS)
	
#c生成.o
$(C_OBJS): %.o: %.c $(HEADS)
	$(CC) -c $(CFLAGS) $(INCS_DIR) $(DEFINE) $(patsubst %.o,%.c,$@)
#cpp生成.o
$(CPP_OBJS): %.o: %.cpp $(HEADS)
	$(CXX) -c $(CFLAGS) $(INCS_DIR) $(DEFINE) $(patsubst %.o,%.cpp,$@)
	
	
#清理文件命令
.PHONY: clean
clean: 
	-rm $(TARGETS) $(C_OBJS) $(CPP_OBJS)

#重构文件命令
.PHONY: rebuild
rebuild: clean $(TARGETS)