#pragma once
//常用头文件和using声明
//20191122-1653


//c库
#include <cstdio>//cio
#include <cassert>//断言
#include <cstddef>//大小类型
#include <cstdlib>//杂项
#include <cctype>//字符分类
#include <cstring>//字符串处理
#include <cmath>//数学
#include <clocale>//本地化
#include <cstdint>//类型扩展
#include <ctime>//时间
#include <cstdarg>//变长参数

//io相关
#include <iostream>//c++io
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::ends;
using std::ios_base;
using std::istream;
using std::ostream;
#include <sstream>//字符串io
using std::istringstream;
using std::ostringstream;
using std::stringstream;
#include <fstream>//文件io
using std::ifstream;
using std::ofstream;
using std::fstream;
#include <iomanip>//高级io

//泛型相关
#include <type_traits>//类型处理
using std::enable_if;
#include<limits>//数值极限
using std::numeric_limits;
#include <numeric>//数值算法
#include <algorithm>//算法
#include <initializer_list>//列表
using std::initializer_list;
#include <utility>//基础容器
using std::swap;
using std::pair;
using std::make_pair;
using std::piecewise_construct;
using std::declval;
#include <tuple>//元组
using std::tuple;
using std::tuple_size;
using std::tuple_element;
using std::make_tuple;
//using std::tie;
using std::forward_as_tuple;
using std::ignore;
#include <functional>//函数
using std::function;
using std::hash;
//using std::ref;
//using std::cref;
namespace placeholders = std::placeholders;
#include <iterator>//迭代器
using std::iterator_traits;
using std::reverse_iterator;
using std::move_iterator;
using std::make_move_iterator;
using std::back_insert_iterator;
using std::back_inserter;
using std::front_insert_iterator;
using std::front_inserter;
using std::insert_iterator;
using std::inserter;

//容器相关
#include <string>//字符串
using std::string;
using std::wstring;
using std::u16string;
using std::u32string;
#include <vector>//向量
using std::vector;
#include <array>//数组
using std::array;
#include <deque>//双向向量
using std::deque;
#include <list>//链表
using std::list;
#include <set>//稳定排序二叉树
//using std::set;
using std::multiset;
#include <map>//稳定排序二叉树对
//using std::map;
using std::multimap;
#include <unordered_set>//哈希表
using std::unordered_set;
using std::unordered_multiset;
#include <unordered_map>//哈希表对
using std::unordered_map;
using std::unordered_multimap;
#include <queue>//队列
using std::priority_queue;

//工具类相关
#include <memory>//内存
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
//using std::make_unique;
#include <exception>//异常
using std::exception;
#include <stdexcept>//更多异常
using std::runtime_error;
#include <random>//随机数
using std::default_random_engine;
using std::mt19937;
using std::mt19937_64;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::discrete_distribution;
#include <ratio>//比例
using std::ratio;
#include <complex>//复数
using std::complex;
#include <regex>//正则表达式
using std::regex;
using std::wregex;
using std::regex_error;
using std::cmatch;
using std::smatch;
using std::wcmatch;
using std::wsmatch;
using std::cregex_iterator;
using std::sregex_iterator;
using std::wcregex_iterator;
using std::wsregex_iterator;
using std::csub_match;
using std::ssub_match;
using std::wcsub_match;
using std::wssub_match;

//跨平台相关
#include <locale>//本地化
#include <chrono>//时钟
using std::chrono::time_point;
using std::chrono::time_point_cast;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::system_clock;
using std::chrono::steady_clock;
namespace chrono = std::chrono;
#include <thread>//线程
using std::thread;
namespace this_thread = std::this_thread;
#include <mutex>//互斥
using std::mutex;
using std::timed_mutex;
using std::recursive_mutex;
using std::recursive_timed_mutex;
using std::lock_guard;
using std::unique_lock;
using std::defer_lock;
using std::try_to_lock;
using std::adopt_lock;
using std::once_flag;
#include <condition_variable>//线程同步
using std::condition_variable;
using std::condition_variable_any;
using std::cv_status;
#include <future>//期货
using std::future;
using std::shared_future;
#include <atomic>//原子库
using std::atomic;
using std::atomic_flag;
using std::memory_order;
using std::memory_order_relaxed;
using std::memory_order_consume;
using std::memory_order_acquire;
using std::memory_order_release;
using std::memory_order_acq_rel;
using std::memory_order_seq_cst;




//标准库扩展
#include "UsedMacro.h"
#include "TypeExtend.h"
#include "IoOperator.h"
#include "StruAndAlgo.h"
