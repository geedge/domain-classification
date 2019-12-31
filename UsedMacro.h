#pragma once

//³£ÓÃºê¶¨Òå¿â
//20191111-1705

#include <cstdio>
#include <cassert>

#include <iostream>

#include <type_traits>
#include <utility>

#include <string>

#include <exception>
#include <stdexcept>


//**********
//±êÊ¶·û²Ù×÷
//**********

#define SYMBOL_STR(id) #id//×ª±ä×Ö·û´®



#define CONNECT_SYMBOL_2(str1, str2) str1##str2
#define CONNECT_SYMBOL_3(str1, str2, str3) str1##str2##str3
#define CONNECT_SYMBOL_4(str1, str2, str3, str4) str1##str2##str3##str4



//************
//ÊýÖµÔËËã²Ù×÷
//************

#define NEG_TO(x) ((x) = -(x))//¸³Ïà·´Êý
#define NOT_TO(x) ((x) = !(x))//¸³²¼¶û·Ç
#define INV_TO(x) ((x) = ~(x))//¸³°´Î»È¡·´


#define ABS_OF(x) ((x)>0? (x):-(x))//È¡¾ø¶ÔÖµ
#define ABS_TO(x) ((x) = ABS_OF(x))//¸³¾ø¶ÔÖµ


#define DIS_OF(x, y) (ABS_OF((x)-(y)))//È¡²îÖµ



#define MAX_OF(x, y) ((x)>(y)? (x):(y))//È¡×î´óÖµ
#define MIN_OF(x, y) ((x)<(y)? (x):(y))//È¡×îÐ¡Öµ


#define MAX_TO(x, lim) ((x) = MAX_OF(x, lim))//¸³×î´óÖµ
#define MIN_TO(x, lim) ((x) = MIN_OF(x, lim))//¸³×îÐ¡Öµ

#define HIGH_LIMIT(x, lim) MIN_TO(x, lim)//ÉèÖÃÉÏÏÞ
#define LOW_LIMIT(x, lim) MAX_TO(x, lim)//ÉèÖÃÏÂÏÞ


#define RANGE_OF(x, llim, hlim) ((x)<(llim)? (llim): (x)>(hlim)? (hlim):(x))//È¡·¶Î§
#define RANGE_LIMIT(x, llim, hlim) ((x) = RANGE_OF(x, llim, hlim))//ÉèÖÃ·¶Î§


#define IS_IN_RANGE(x, llim, hlim) ((x)>=(llim) && (x)<=(hlim))//ÅÐ¶ÏÊÇ·ñÔÚ·¶Î§ÄÚ
#define IS_IN_ITER_RANGE(x, st, ed) ((x)>=(st) && (x)<(ed))//ÅÐ¶ÏÊÇ·ñÔÚ·¶Î§ÄÚ£¬ÒÔµü´úÆ÷±ê×¼



#define SQUARE_OF(x) ((x)*(x))//ÇóÆ½·½
#define SQUARE_TO(x) ((x) = SQUARE_OF(x))//¸³ÖµÆ½·½



//************
//ÔËËãÊ¡ÂÔ²Ù×÷
//************



#define LIST_GO(p, m_p) ((p) = (p)->m_p)//Á´±íÖ¸ÕëÒÆ¶¯

#define LIST_CNNT(p1, m_p1, p2, m_p2) \
	(((p1)->m_p1=p2), ((p2)->m_p2=p1))//Ë«ÏòÁ´±íÁ¬½Ó

#define LIST_ADD(p1, m_p1, p2, m_p2, pNew) \
	(LIST_CNNT(p1, m_p1, pNew, m_p2), LIST_CNNT(pNew, m_p1, p2, m_p2))//Ë«ÏòÁ´±íÌí¼Ó



#define ARR_SIZE(arr) (sizeof(decltype(arr))\
	/sizeof(typename std::remove_extent<decltype(arr)>::type))//ÇóÊý×é´óÐ¡

#define ARR_SIZE_ALL(arr) (sizeof(decltype(arr))\
	/sizeof(typename std::remove_all_extents<decltype(arr)>::type))//Çó¸ùÊý×é´óÐ¡



#define SHOW_BEGIN_END(container) std::begin(container), std::end(container)//ÁÐ³öÈÝÆ÷µÄbeginºÍendº¯Êý



#define O_(id) CONNECT_SYMBOL_2(o_, id)//Ìí¼ÓÔ­Ê¼Ç°×º

#define O_INIT(id) id(O_(id))//Ê¹ÓÃÇ°×º°æ±¾³õÊ¼»¯
#define O_INIT_MOVE(id) id(std::move(O_(id)))//Ê¹ÓÃÇ°×º°æ±¾ÒÆ¶¯³õÊ¼»¯
#define O_INIT_FORWARD(id, Ty) id(std::forward<Ty>(O_(id)))//Ê¹ÓÃÇ°×º°æ±¾×ª·¢³õÊ¼»¯

#define O_ASSIGN(id) (id = O_(id))//Ê¹ÓÃÇ°×º°æ±¾¸³Öµ
#define O_ASSING_MOVE(id) (id = std::move(O_(id)))//Ê¹ÓÃÇ°×º°æ±¾ÒÆ¶¯¸³Öµ
#define O_ASSIGN_FORWARD(id, Ty) (id = std::forward<Ty>(O_(id)))//Ê¹ÓÃÇ°×º°æ±¾×ª·¢³õÊ¼»¯


#define OTHER_INIT(other, mem) mem((other).mem)//Ê¹ÓÃÀà³ÉÔ±³õÊ¼»¯
#define OTHER_INIT_MOVE(other, mem) mem(std::move(other).mem)//Ê¹ÓÃÀà³ÉÔ±ÒÆ¶¯³õÊ¼»¯
#define OTHER_INIT_FORWARD(other, mem, Ty) mem(std::forward<Ty>(other).mem)//Ê¹ÓÃÀà³ÉÔ±×ª·¢³õÊ¼»¯

#define OTHER_ASSIGN(other, mem) (mem = (other).mem)//Ê¹ÓÃÀà³ÉÔ±¸³Öµ
#define OTHER_ASSIGN_MOVE(other, mem) (mem = std::move(other).mem)//Ê¹ÓÃÀà³ÉÔ±ÒÆ¶¯¸³Öµ
#define OTHER_ASSIGN_FORWARD(other, mem) (mem = std::forward<Ty>(other).mem)//Ê¹ÓÃÀà³ÉÔ±×ª·¢¸³Öµ



//******
//¶àÓï¾ä²Ù×÷
//******

#define CLASS_ASSIGN(Type, other) {\
	if(this!=&(other)) {\
		this->~Type();\
		new(this) Type(other);\
	}}//¿½±´¸³Öµ£¬×¢Òâ»á¸²¸ÇÐé±í£¬²»½¨ÒéÊ¹ÓÃ

#define CLASS_ASSIGN_MOVE(Type, other) {\
	if(this!=&(other)) {\
		this->~Type();\
		new(this) Type(std::move(other));\
	}}//ÒÆ¶¯¸³Öµ£¬×¢Òâ»á¸²¸ÇÐé±í£¬²»½¨ÒéÊ¹ÓÃ


#define TEMPLATE_ASSIGN(Type, Typename, arg) {\
	this->~Type();\
	new(this) Type(std::forward<Typename>(arg));\
	}//Ä£°å¸³Öµ£¬×Ô¸³Öµ²»°²È«£¬×¢Òâ»á¸²¸ÇÐé±í£¬²»½¨ÒéÊ¹ÓÃ

#define TEMPLATE_PACKET_ASSIGN(Type, Typenames, args) {\
	this->~Type();\
	new(this) Type(std::forward<Typenames>(args)...);\
	}//Ä£°å°ü¸³Öµ£¬×Ô¸³Öµ²»°²È«£¬×¢Òâ»á¸²¸ÇÐé±í£¬²»½¨ÒéÊ¹ÓÃ



//******
//ÉùÃ÷²Ù×÷
//******

#define DEFINE_INLINE_VERIABLE(Type, name) \
inline Type &name() \
{\
	static Type value;\
	return value;\
}//¶¨ÒåÄÚÁª±äÁ¿

#define GET_INLINE_VARIABLE(name) name()//»ñÈ¡ÄÚÁª±äÁ¿

