#pragma once

//±ê×¼¿âÀàÐÍÀ©Õ¹²Ù×÷
//20191212-1029

#include <cassert>
#include <cmath>
#include <cstdint>

#include <type_traits>
#include <limits>
#include <algorithm>
#include <utility>
#include <tuple>
#include <functional>
#include <iterator>

#include <string>

#include <exception>
#include <stdexcept>
#include <random>
#include <ratio>

#include <chrono>




//************
//ÀàÐÍ±ðÃûÀ©Õ¹
//************


//durationÀ©Õ¹
namespace std
{
namespace chrono
{
typedef duration<long, ratio<3600*24>> days;
typedef duration<long, ratio<3600*24*7>> weeks;
}
}



//************
//»ù´¡ÀàÐÍÀ©Õ¹
//************

//ÎÞÓÃÕ¼Î»ÀàÐÍ
template<int N>
class BlankType
{
};



//²âÊÔ²ÎÊýÊÇ·ñºÏ·¨Àà
template<typename ...>
struct ParamValidTester
{
	typedef void type;
};


//ÏÈÈ¥³ýÒýÓÃÔÙÈ¥³ý³£ÊýÒ×±äÊôÐÔ
template<typename Ty>
struct RemoveCVRef
{
	using type = typename std::remove_cv<typename std::remove_reference<Ty>::type>::type;
};

//ºóÕßÈ¥³ýÒýÓÃ³£ÊýÒ×±äºóÊÇ·ñÎªÇ°Õß
template<typename TyDst, typename TySrc>
struct IsRemoveCVRefSame:
	std::is_same<TyDst, typename RemoveCVRef<TySrc>::type>
{
};


//ºóÕß²ÎÊý°üÊÇ·ñÒª²»Ã»ÓÐ²ÎÊý£¬Òª²»Ö»ÓÐÒ»¸ö²¢ÇÒÎªÇ°Õß
template<typename TyDst, typename ...TySrc_S>
struct IsNoneOrSame:
	std::integral_constant<bool, sizeof...(TySrc_S)==0>
{
};
template<typename TyDst, typename TySrc>
struct IsNoneOrSame<TyDst, TySrc>:
	std::is_same<TyDst, TySrc>
{
};

//ºóÕß²ÎÊý°üÊÇ·ñÒª²»Ã»ÓÐ²ÎÊý£¬Òª²»Ö»ÓÐÒ»¸ö²¢ÇÒÈ¥³ýÒýÓÃ³£ÊýÒ×±äºóÎªÇ°Õß
template<typename TyDst, typename ...TySrc_S>
struct IsNoneOrRemoveCVRefSame:
	std::integral_constant<bool, sizeof...(TySrc_S)==0>
{
};
template<typename TyDst, typename TySrc>
struct IsNoneOrRemoveCVRefSame<TyDst, TySrc>:
	IsRemoveCVRefSame<TyDst, TySrc>
{
};


//²âÊÔÊÇ·ñÎªµü´úÆ÷
template<class Ty, class= void>
struct IsIteratorType:
	std::false_type
{
};
template<class Ty>
struct IsIteratorType<Ty, typename ParamValidTester<
	typename std::iterator_traits<Ty>::iterator_category>::type
>: std::true_type
{
};


//²âÊÔÈ¥³ý³£Ò×±äÒ×ÓÃÊôÐÔºó£¬ÊÇ·ñÎª×Ö·ûÖ¸Õë£¬×Ö·ûÊý×é»òstringÀàÐÍ
template<class Ty>
struct IsRemoveCVRefSameSzOrString:
	std::integral_constant<bool, IsRemoveCVRefSame<char *, Ty>::value
	|| IsRemoveCVRefSame<const char *, Ty>::value
	|| (std::is_array<typename RemoveCVRef<Ty>::type>::value
	&& IsRemoveCVRefSame<char, typename std::remove_extent<
	typename RemoveCVRef<Ty>::type>::type>::value)
	|| IsRemoveCVRefSame<std::string, Ty>::value>
{
};



//Éú³Éµü´úÆ÷½âÒýÓÃµÄÀàÐÍ£¬±£Áô³£Ò×±äÒýÓÃÊôÐÔ
template<typename TyIt>
struct IteratorDerefType
{
	using type = decltype(*std::declval<TyIt>());
};

//Éú³Éµü´úÆ÷½âÒýÓÃµÄÀàÐÍÔÙÈ¥³ý³£Ò×±äÒýÓÃÊôÐÔ
template<typename TyIt>
struct IteratorRemoveCVRefDerefType
{
	using type = typename RemoveCVRef<typename IteratorDerefType<TyIt>::type>::type;
};



//¸ù¾ÝÌá¹©À´Ô´ÀàÐÍµÄÒýÓÃ¡¢³£Êý¡¢Ò×±äÊôÐÔ£¬¶ÔÄ¿µÄÀàÐÍ¸½¼ÓÍ¬ÑùµÄÊôÐÔ
template<typename TySrc, typename TyDst>
struct TypeAttachAttribute
{
	using type = TyDst;
};
//³£ÊôÐÔÌØ»¯
template<typename TySrc, typename TyDst>
struct TypeAttachAttribute<const TySrc, TyDst>
{
	using type = typename std::add_const<
		typename TypeAttachAttribute<TySrc, TyDst>::type>::type;
};
//Ò×±äÊôÐÔÌØ»¯
template<typename TySrc, typename TyDst>
struct TypeAttachAttribute<volatile TySrc, TyDst>
{
	using type = typename std::add_volatile<
		typename TypeAttachAttribute<TySrc, TyDst>::type>::type;
};
//×óÖµÒýÓÃÊôÐÔÌØ»¯
template<typename TySrc, typename TyDst>
struct TypeAttachAttribute<TySrc &, TyDst>
{
	using type = typename std::add_lvalue_reference<
		typename TypeAttachAttribute<TySrc, TyDst>::type>::type;
};
//ÓÒÖµÒýÓÃÊôÐÔÌØ»¯
template<typename TySrc, typename TyDst>
struct TypeAttachAttribute<TySrc &&, TyDst>
{
	using type = typename std::add_rvalue_reference<
		typename TypeAttachAttribute<TySrc, TyDst>::type>::type;
};



//Çóµ÷ÓÃ·µ»ØÖµÀàÐÍ
template<typename TyFunc, typename ...Ty_S>
struct InvokeReturnType
{
	using type = decltype(std::declval<TyFunc>()(std::declval<Ty_S>()...));
};



//ÕûÊýÐòÁÐÄ£°å
template<size_t ...c_idx_s>
struct IndexSequence
{
};
template<>
struct IndexSequence<>
{
	using value_type = size_t;
	static constexpr size_t size()
	{
		return 0;
	}
};
template<int c_idx, int ...c_rest_s>
struct IndexSequence<c_idx, c_rest_s...>
{
	using value_type = size_t;
	static constexpr size_t size()
	{
		return IndexSequence<c_rest_s...>::size()+1;
	}
};

//ÕûÊýÄ£°åÌí¼ÓÔªËØº¯Êý
template<size_t c_new, size_t ...c_idx_s>
inline IndexSequence<c_idx_s..., c_new> AddIndexSequence(IndexSequence<c_idx_s...>)
{
	return IndexSequence<c_idx_s..., c_new>();
}

//Éú³ÉË³ÐòµÝÔöÕûÊýÄ£°å¸¨ÖúÀà
template<size_t c_size>
struct _MakeIndexSequenceAssist
{
	using type = decltype(AddIndexSequence<c_size-1>(
		typename _MakeIndexSequenceAssist<c_size-1>::type()));
};
template<>
struct _MakeIndexSequenceAssist<0>
{
	using type = IndexSequence<>;
};
//Éú³ÉË³ÐòµÝÔöÕûÊýÄ£°åÀà
template<size_t c_size>
using MakeIndexSequence = typename _MakeIndexSequenceAssist<c_size>::type;



//Ä£°å¶¨³¤ÕûÐÎ
template<bool c_bSigned, int c_size>
struct TemplateInt
{
};
template<>
struct TemplateInt<true, 1>
{
	using type = int8_t;
};
template<>
struct TemplateInt<false, 1>
{
	using type = uint8_t;
};
template<>
struct TemplateInt<true, 2>
{
	using type = int16_t;
};
template<>
struct TemplateInt<false, 2>
{
	using type = uint16_t;
};
template<>
struct TemplateInt<true, 4>
{
	using type = int32_t;
};
template<>
struct TemplateInt<false, 4>
{
	using type = uint32_t;
};
template<>
struct TemplateInt<true, 8>
{
	using type = int64_t;
};
template<>
struct TemplateInt<false, 8>
{
	using type = uint64_t;
};



//¹ãÒåµÄ±È½Ïº¯ÊýÀà
template<typename Ty1, typename Ty2= Ty1>
struct GeneralLess
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return arg1<arg2;
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct GeneralEqualTo
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return arg1==arg2;
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct GeneralLessEqual
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return arg1<=arg2;
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct GeneralGreater
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return arg1>arg2;
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct GeneralGreaterEqual
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return arg1>=arg2;
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct GeneralNotEqualTo
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return arg1!=arg2;
	}
};


//À©Õ¹µÄ¹ãÒå±È½Ïº¯ÊýÀà£¬Ê¹ÓÃÐ¡ÓÚºÍµÈÓÚ²Ù×÷ÔËËã
template<typename Ty1, typename Ty2= Ty1>
struct ExtendGeneralLessEqual
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return arg1<arg2 || arg1==arg2;
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct ExtendGeneralGreater
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return !(arg1<arg2 || arg1==arg2);
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct ExtendGeneralGreaterEqual
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return !(arg1<arg2);
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct ExtendGeneralNotEqualTo
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return !(arg1==arg2);
	}
};


//±È½ÏÀàÐÍµÄº¯Êý·â×°
template<typename Ty1, typename Ty2= Ty1>
using CompareType = std::function<bool(const Ty1 &, const Ty2 &)>;


//±È½ÏÀàº¯ÊýÀ©Õ¹£¬Ê¹ÓÃÐ¡ÓÚºÍµÈÓÚº¯Êý
template<typename Ty1, typename Ty2= Ty1>
struct ExtendLessEqualWrap
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
public:
	CompareType<Ty1, Ty2> funcLT;
	CompareType<Ty1, Ty2> funcEQ;
public:
	explicit ExtendLessEqualWrap(const CompareType<Ty1, Ty2> &o_funcLT,
		const CompareType<Ty1, Ty2> &o_funcEQ)
		:
		funcLT(o_funcLT), funcEQ(o_funcEQ)
	{
	}
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return funcLT(arg1, arg2) || funcEQ(arg1, arg2);
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct ExtendGreaterWrap
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
public:
	CompareType<Ty1, Ty2> funcLT;
	CompareType<Ty1, Ty2> funcEQ;
public:
	explicit ExtendGreaterWrap(const CompareType<Ty1, Ty2> &o_funcLT,
		const CompareType<Ty1, Ty2> &o_funcEQ)
		:
		funcLT(o_funcLT), funcEQ(o_funcEQ)
	{
	}
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return !(funcLT(arg1, arg2) || funcEQ(arg1, arg2));
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct ExtendGreaterEqualWrap
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
public:
	CompareType<Ty1, Ty2> funcLT;
	CompareType<Ty1, Ty2> funcEQ;
public:
	explicit ExtendGreaterEqualWrap(const CompareType<Ty1, Ty2> &o_funcLT,
		const CompareType<Ty1, Ty2> &o_funcEQ)
		:
		funcLT(o_funcLT), funcEQ(o_funcEQ)
	{
	}
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return !funcLT(arg1, arg2);
	}
};
template<typename Ty1, typename Ty2= Ty1>
struct ExtendNotEqualToWrap
{
	typedef bool result_type;
	typedef Ty1 first_argument_type;
	typedef Ty2 second_argument_type;
public:
	CompareType<Ty1, Ty2> funcLT;
	CompareType<Ty1, Ty2> funcEQ;
public:
	explicit ExtendNotEqualToWrap(const CompareType<Ty1, Ty2> &o_funcLT,
		const CompareType<Ty1, Ty2> &o_funcEQ)
		:
		funcLT(o_funcLT), funcEQ(o_funcEQ)
	{
	}
	result_type operator()(const first_argument_type &arg1,
		const second_argument_type &arg2) const
	{
		return !funcEQ(arg1, arg2);
	}
};



//´æ´¢Ä³ÀàÐÍ£¬¿ÉÒÔÎªÖµ»òÒýÓÃ£¬¶Ô¸÷ÖÖÇé¿öÒÑ½øÐÐÌØ»¯
//ÖµÀàÐÍÌØ»¯
template<typename Ty>
struct TypeHolder
{
	Ty hold;
public:
	//¹¹Ôì
	TypeHolder(const Ty &o_hold):
		hold(o_hold)
	{
	}
	TypeHolder(Ty &&o_hold):
		hold(std::move(o_hold))
	{
	}
};
//×óÖµÒýÓÃÌØ»¯
template<typename Ty>
struct TypeHolder<Ty &>
{
	Ty &hold;
public:
	//¹¹Ôì
	TypeHolder(Ty &o_hold):
		hold(o_hold)
	{
	}
};
//ÓÒÖµÒýÓÃÌØ»¯
template<typename Ty>
struct TypeHolder<Ty &&>
{
	Ty &&hold;
public:
	//¹¹Ôì
	TypeHolder(Ty &&o_hold):
		hold(std::move(o_hold))
	{
	}
};



//´æ´¢Ä³ÖµÀàÐÍ£¬²»ÄÜÎªÒýÓÃ£¬ÀàÐÍÖ§³Övoid
template<typename Ty>
struct ValueHolder
{
public:
	using RefType = Ty &;
	using ConstRefType = const Ty &;
	using PtrType = Ty *;
	using ConstPtrType = const Ty *;
public:
	Ty hold;
public:
	//Ä£°å¹¹Ôì
	template<typename ...Ty_S>
	explicit ValueHolder(Ty_S &&...args):
		hold(std::forward<Ty_S>(args)...)
	{
	}
};
//voidÌØ»¯
template<>
struct ValueHolder<void>
{
	using RefType = void;
	using ConstRefType = void;
	using PtrType = void;
	using ConstPtrType = void;
};



//**********************
//ÀàÐÍ³ÉÔ±»ò¸¨Öúº¯ÊýÀ©Õ¹
//**********************

//µ÷ÓÃ±äÁ¿µÄÎö¹¹º¯Êý
template<typename Ty>
inline void CallDestruct(Ty &value)
{
	value.~Ty();
}

//µ÷ÓÃ±äÁ¿µÄ¹¹Ôìº¯Êý£¬ÒªÇó±äÁ¿ÒÑ¾­Îö¹¹
template<typename Ty, typename ...Ty_S>
inline void CallConstruct(Ty &value, Ty_S &&...arg_s)
{
	new(&value) Ty(std::forward<Ty_S>(arg_s)...);
}

//µ÷ÓÃ±äÁ¿µÄÎö¹¹º¯ÊýºÍ¹¹Ôìº¯Êý
template<typename Ty, typename ...Ty_S>
inline void CallRestruct(Ty &value, Ty_S &&...arg_s)
{
	CallDestruct(value);
	CallConstruct(value, std::forward<Ty_S>(arg_s)...);
}



//ÌáÈ¡±äÁ¿µÄ×óÖµÒýÓÃ
template<typename Ty>
inline Ty &GetLref(Ty &&value)
{
	return value;
}
//ÌáÈ¡±äÁ¿µÄ³£×óÖµÒýÓÃ
template<typename Ty>
inline const Ty &GetConstLref(Ty &&value)
{
	return value;
}


//¶Ô±äÁ¿½øÐÐÖµ¿½±´
template<typename Ty>
inline Ty GetValueCopy(const Ty &value)
{
	return Ty(value);
}



//c·ç¸ñ×Ö·û´®×ª»¯×Ö·û´®£¬×Ö·ûÖ¸ÕëÖØÔØ
inline std::string OverrideSzToStr(const char *sz)
{
	return std::string(sz);
}
inline std::string OverrideSzToStr(char *sz)
{
	return std::string(sz);
}
//c·ç¸ñ×Ö·û´®×ª»¯×Ö·û´®£¬×Ö·ûÊý×éÖØÔØ
template<typename Ty>
inline typename std::enable_if<std::is_array<typename RemoveCVRef<Ty>::type>::value
	&& IsRemoveCVRefSame<char, typename std::remove_extent<
	typename RemoveCVRef<Ty>::type>::type>::value, std::string
>::type OverrideSzToStr(Ty &&sz)
{
	return std::string(std::forward<Ty>(sz));
}
//c·ç¸ñ×Ö·û´®×ª»¯×Ö·û´®£¬ÆäÓàÇé¿öÖØÔØ
template<typename Ty>
inline Ty &&OverrideSzToStr(Ty &&arg)
{
	return std::forward<Ty>(arg);
}



//ÕûÐÎ±È½Ï¸¨Öúºê
#define _IS_INT_TYPE(Ty1, Ty2) \
 (std::is_integral<Ty1>::value && std::is_integral<Ty2>::value)
#define _INT_SIGN_UNSIGN(Ty1, Ty2) \
	((std::is_integral<Ty1>::value && std::is_signed<Ty1>::value)\
	&& std::is_unsigned<Ty2>::value)

//ÕûÐÎÀàÐÍ°²È«ÅÐ¶ÏÐ¡ÓÚ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_INT_SIGN_UNSIGN(Ty1, Ty2), bool
>::type IntLT(Ty1 num1, Ty2 num2)
{
	return (num1<0) ? (true)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)<num2);
}
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_INT_SIGN_UNSIGN(Ty2, Ty1), bool
>::type IntLT(Ty1 num1, Ty2 num2)
{
	return (num2<0) ? (false)
		: (num1<static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_INT_TYPE(Ty1, Ty2)
	&& !_INT_SIGN_UNSIGN(Ty1, Ty2) && !_INT_SIGN_UNSIGN(Ty2, Ty1), bool
>::type IntLT(Ty1 num1, Ty2 num2)
{
	return num1<num2;
}

//ÕûÐÎÀàÐÍ°²È«ÅÐ¶ÏµÈÓÚ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_INT_SIGN_UNSIGN(Ty1, Ty2), bool
>::type IntEQ(Ty1 num1, Ty2 num2)
{
	return (num1<0) ? (false)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)==num2);
}
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_INT_SIGN_UNSIGN(Ty2, Ty1), bool
>::type IntEQ(Ty1 num1, Ty2 num2)
{
	return (num2<0) ? (false)
		: (num1==static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_INT_TYPE(Ty1, Ty2)
	&& !_INT_SIGN_UNSIGN(Ty1, Ty2) && !_INT_SIGN_UNSIGN(Ty2, Ty1), bool
>::type IntEQ(Ty1 num1, Ty2 num2)
{
	return num1==num2;
}

//ÕûÐÎÀàÐÍ°²È«ÅÐ¶ÏÐ¡ÓÚµÈÓÚ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_INT_TYPE(Ty1, Ty2), bool
>::type IntLTE(Ty1 num1, Ty2 num2)
{
	return IntLT(num1, num2) || IntEQ(num1, num2);
}

//ÕûÐÎÀàÐÍ°²È«ÅÐ¶Ï´óÓÚ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_INT_TYPE(Ty1, Ty2), bool
>::type IntGT(Ty1 num1, Ty2 num2)
{
	return !(IntLT(num1, num2) || IntEQ(num1, num2));
}

//ÕûÐÎÀàÐÍ°²È«ÅÐ¶Ï´óÓÚµÈÓÚ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_INT_TYPE(Ty1, Ty2), bool
>::type IntGTE(Ty1 num1, Ty2 num2)
{
	return !IntLT(num1, num2);
}

//ÕûÐÎÀàÐÍ°²È«ÅÐ¶Ï²»µÈÓÚ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_INT_TYPE(Ty1, Ty2), bool
>::type IntNEQ(Ty1 num1, Ty2 num2)
{
	return !IntEQ(num1, num2);
}
#undef _IS_INT_TYPE
#undef _INT_SIGN_UNSIGN



//¸¡µãÊý±È½Ï¸¨Öúºê
#define _FLOAT_DEFAULT_ULP 10
#define _FLOAT_DEFAULT_ABS_MODE true
#define _IS_FLOAT_TYPE(Ty1, Ty2) \
	(std::is_floating_point<Ty1>::value && std::is_floating_point<Ty2>::value)
#define _FLOAT_COMMON_EPSILON(Ty1, Ty2) \
	(std::max(std::numeric_limits<Ty1>::epsilon(), std::numeric_limits<Ty2>::epsilon()))
#define _FLOAT_COMMON_MIN(Ty1, Ty2) \
	(std::max(std::numeric_limits<Ty1>::min(), std::numeric_limits<Ty2>::min()))
#define _FLOAT_ABS(num) ((num)>=0? (num):-(num))

//¸¡µãÊý±È½ÏÏàµÈ£¬±£Ö¤Âß¼­¹ØÏµ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_FLOAT_TYPE(Ty1, Ty2), bool
>::type FloatEQ(Ty1 num1, Ty2 num2, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	return bAbs ?
		(_FLOAT_ABS(num1-num2)<_FLOAT_COMMON_EPSILON(Ty1, Ty2)*ulp)
		:
		(_FLOAT_ABS(num1-num2)<_FLOAT_COMMON_EPSILON(Ty1, Ty2)
		*(_FLOAT_ABS(num1)+_FLOAT_ABS(num2))*ulp
		|| _FLOAT_ABS(num1-num2)<_FLOAT_COMMON_MIN(Ty1, Ty2)*ulp);
}

//¸¡µãÊý±È½ÏÐ¡ÓÚ£¬±£Ö¤Âß¼­¹ØÏµ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_FLOAT_TYPE(Ty1, Ty2), bool
>::type FloatLT(Ty1 num1, Ty2 num2, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	return num1<num2 && !FloatEQ(num1, num2, bAbs, ulp);
}

//¸¡µãÊý±È½ÏÐ¡ÓÚµÈÓÚ£¬±£Ö¤Âß¼­¹ØÏµ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_FLOAT_TYPE(Ty1, Ty2), bool
>::type FloatLTE(Ty1 num1, Ty2 num2, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	return FloatLT(num1, num2, bAbs, ulp) || FloatEQ(num1, num2, bAbs, ulp);
}

//¸¡µãÊý±È½Ï´óÓÚ£¬±£Ö¤Âß¼­¹ØÏµ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_FLOAT_TYPE(Ty1, Ty2), bool
>::type FloatGT(Ty1 num1, Ty2 num2, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	return !(FloatLT(num1, num2, bAbs, ulp) || FloatEQ(num1, num2, bAbs, ulp));
}

//¸¡µãÊý±È½Ï´óÓÚµÈÓÚ£¬±£Ö¤Âß¼­¹ØÏµ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_FLOAT_TYPE(Ty1, Ty2), bool
>::type FloatGTE(Ty1 num1, Ty2 num2, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	return !FloatLT(num1, num2, bAbs, ulp);
}

//¸¡µãÊý±È½Ï²»µÈÓÚ£¬±£Ö¤Âß¼­¹ØÏµ
template<typename Ty1, typename Ty2>
inline constexpr typename std::enable_if<_IS_FLOAT_TYPE(Ty1, Ty2), bool
>::type FloatNEQ(Ty1 num1, Ty2 num2, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	return !FloatEQ(num1, num2, bAbs, ulp);
}


//¸¡µãÊýÏÂ×ª»¯ÕûÊý
template<typename Ty>
inline typename std::enable_if<std::is_floating_point<Ty>::value, Ty
>::type FloatFloor(Ty num, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	Ty res = std::round(num);
	if(FloatEQ(num, res, bAbs, ulp))
		return res;
	return std::floor(num);
}

//¸¡µãÊýÉÏ×ª»¯ÕûÊý
template<typename Ty>
inline typename std::enable_if<std::is_floating_point<Ty>::value, Ty
>::type FloatCeil(Ty num, bool bAbs= _FLOAT_DEFAULT_ABS_MODE, int ulp= _FLOAT_DEFAULT_ULP)
{
	Ty res = std::round(num);
	if(FloatEQ(num, res, bAbs, ulp))
		return res;
	return std::ceil(num);
}
#undef _FLOAT_DEFAULT_ULP
#undef _IS_FLOAT_TYPE
#undef _FLOAT_COMMON_EPSILON
#undef _FLOAT_COMMON_MIN
#undef _FLOAT_ABS



//stringµÄÀÛ¼Ó²Ù×÷£¬Ê¹ÓÃ×óÒÆÔËËã·û
template<typename Ele, typename Ty>
inline std::string &operator <<(std::basic_string<Ele> &str, Ty &&arg)
{
	return str += std::forward<Ty>(arg);
}



//·¶Î§ÄÚ×ÖµäÐò±È½Ï²Ù×÷£¬Ê¹ÓÃµü´úÆ÷£¬Ð¡ÓÚµÈÓÚ´óÓÚ·Ö±ð·µ»Ø-1,0,1
template<typename TyIt1, typename TyIt2,
	typename TyLess= GeneralLess<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>,
	typename TyEqual= GeneralEqualTo<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>
> inline int SequenceCompare(TyIt1 st1, TyIt1 ed1, TyIt2 st2, TyIt2 ed2,
	TyLess funcLess= TyLess(), TyEqual funcEqual= TyEqual())
{
	for(; ; ++st1, ++st2) {
		//ÅÐ¶Ï½áÊøÌõ¼þ
		if(st1==ed1) {
			if(st2==ed2)
				return 0;
			else
				return -1;
		}
		else if(st2==ed2)
			return 1;
		//ÅÐ¶Ï´óÐ¡
		if(funcLess(*st1, *st2))
			return -1;
		else if(!(funcEqual(*st1, *st2)))
			return 1;
	}
}



//¼ì²éÊÇ·ñÎªtupleÀà
template<typename Ty>
struct IsTupleType:
	std::false_type
{
};
template<typename ...Ty_S>
struct IsTupleType<std::tuple<Ty_S...>>:
	std::true_type
{
};


//tupleÀàÐÍ×ª»»À©Õ¹¸¨Öúº¯Êý
template<typename TyDst, typename TySrc, size_t ...c_idx_s>
inline TyDst _TupleCastAssist(TySrc &&src, IndexSequence<c_idx_s...>)
{
	return TyDst(std::get<c_idx_s>(std::forward<TySrc>(src))...);
}
//tupleÀàÐÍ×ª»»À©Õ¹£¬Ö±½ÓÍêÃÀ×ª·¢ÒÔÖ§³ÖËùÓÐµÄÒýÓÃÇé¿ö£¬ÈôÐèÓÒÖµÔò½«tupleÒÔÓÒÖµ´«Èë
//Ô´ÀàÐÍÐèÒªÄÜ±»getº¯Êý²ð½â£¬Ä¿µÄÀàÐÍÐèÒªÄÜ½ÓÊÜÔ´ÀàÐÍ²ð½âºóµÄ¹¹Ôìº¯Êý
template<typename TyDst, typename TySrc>
inline TyDst TupleCast(TySrc &&src)
{
	return _TupleCastAssist<TyDst>(std::forward<TySrc>(src), MakeIndexSequence<
		std::tuple_size<typename std::remove_reference<TySrc>::type>::value>());
}



//tupleÀàÐÍµ÷ÓÃº¯Êý·µ»ØÖµÀàÐÍ¸¨ÖúÀàÐÍ
template<typename TyFunc, typename TyTup, typename TyIdx>
struct _TupleInvokeReturnTypeAssist
{
};
template<typename TyFunc, typename TyTup, size_t ...c_idx_s>
struct _TupleInvokeReturnTypeAssist<TyFunc, TyTup, IndexSequence<c_idx_s...>>
{
	using type = decltype(std::declval<TyFunc>()(std::get<c_idx_s>(std::declval<TyTup>())...));
};
//tupleÀàÐÍµ÷ÓÃº¯Êý·µ»ØÖµÀàÐÍ£¬Ê¹ÓÃ´øcvrefµÄÀàÐÍ´«Èë
template<typename TyFunc, typename TyTup>
struct TupleInvokeReturnType
{
	using type = typename _TupleInvokeReturnTypeAssist<TyFunc, TyTup, MakeIndexSequence<
		std::tuple_size<typename std::remove_reference<TyTup>::type>::value>>::type;
};

//tupleÀàÐÍµ÷ÓÃº¯Êý¸¨Öúº¯Êý
template<typename TyFunc, typename TyTup, size_t ...c_idx_s>
inline typename TupleInvokeReturnType<TyFunc &&, TyTup &&
>::type _TupleInvokeAssist(TyFunc &&func, TyTup &&tup, IndexSequence<c_idx_s...>)
{
	return std::forward<TyFunc>(func)(std::get<c_idx_s>(std::forward<TyTup>(tup))...);
}
//tupleÀàÐÍµ÷ÓÃº¯Êý£¬±¾ÖÊÉÏÖ»ÒªÄÜ±»getº¯Êý²ð½â¼´¿É£¬ÈôÐèÓÒÖµÔò½«tupleÒÔÓÒÖµ´«Èë
template<typename TyFunc, typename TyTup>
inline typename TupleInvokeReturnType<TyFunc &&, TyTup &&
>::type TupleInvoke(TyFunc &&func, TyTup &&tup)
{
	return _TupleInvokeAssist(std::forward<TyFunc>(func), std::forward<TyTup>(tup),
		MakeIndexSequence<std::tuple_size<typename std::remove_reference<TyTup>::type>::value>());
}



//hashµ÷ÓÃº¯Êý
template<typename Ty>
inline auto HashFunc(Ty &&arg)->
decltype(std::hash<typename RemoveCVRef<Ty>::type>()(std::forward<Ty>(arg)))
{
	return std::hash<typename RemoveCVRef<Ty>::type>()(std::forward<Ty>(arg));
}


//hashµÄÌØÀý»¯
namespace std
{
//¶ÔpairÌØÀý»¯£¬Ã»ÓÐ½»»»²»±äÐÔ£¬Óëtuple¼æÈÝ
template<typename Ty1, typename Ty2>
struct hash<std::pair<Ty1, Ty2>>
{
	typedef size_t result_type;
	typedef std::pair<Ty1, Ty2> argument_type;
	result_type operator()(const argument_type &pr) const
	{
		return HashFunc(HashFunc(pr.first))
			^ HashFunc(pr.second);
	}
};

//¶ÔtupleÌØÀý»¯£¬Ã»ÓÐ½»»»²»±äÐÔ
template<typename ...Tys>
struct hash<std::tuple<Tys...>>
{
	typedef size_t result_type;
	typedef std::tuple<Tys...> argument_type;
	result_type operator()(const argument_type &tup) const
	{
		return Assist<sizeof...(Tys)>(tup);
	}
private:
	template<size_t c_index>
	static typename std::enable_if<c_index>=2, result_type
	>::type Assist(const std::tuple<Tys...> &tup)
	{
		return HashFunc(Assist<c_index-1>(tup))
			^ HashFunc(std::get<c_index-1>(tup));
	}
	template<size_t c_index>
	static typename std::enable_if<c_index==1, result_type
	>::type Assist(const std::tuple<Tys...> &tup)
	{
		return HashFunc(std::get<0>(tup));
	}
	template<size_t c_index>
	static typename std::enable_if<c_index==0, result_type
	>::type Assist(const std::tuple<Tys...> &tup)
	{
		return 0;
	}
};
}


//¸÷Ààµü´úÆ÷hashÀà£¬Î²ºóµü´úÆ÷½âÒýÓÃ²»°²È«
template<typename Ty>
struct IteratorHash
{
	typedef size_t result_type;
	typedef Ty argument_type;
	result_type operator()(argument_type it) const
	{
		return std::hash<decltype(&*it)>()(&*it);
	}
};



//¸÷Ààµü´úÆ÷±È½ÏÀà£¬Î²ºóµü´úÆ÷½âÒýÓÃ²»°²È«
template<typename Ty>
struct IteratorLess
{
	typedef bool result_type;
	typedef Ty first_argument_type;
	typedef Ty second_argument_type;
	result_type operator()(const first_argument_type &it1,
		const second_argument_type &it2) const
	{
		return &*it1<&*it2;
	}
};
template<typename Ty>
struct IteratorEuqalTo
{
	typedef bool result_type;
	typedef Ty first_argument_type;
	typedef Ty second_argument_type;
	result_type operator()(const first_argument_type &it1,
		const second_argument_type &it2) const
	{
		return &*it1==&*it2;
	}
};


//¸÷Ààµü´úÆ÷½âÒýÓÃÖµ±È½ÏÀà
template<typename Ty>
struct IteratorDerefLess
{
	typedef bool result_type;
	typedef Ty first_argument_type;
	typedef Ty second_argument_type;
	result_type operator()(const first_argument_type &it1,
		const second_argument_type &it2) const
	{
		return *it1<*it2;
	}
};
template<typename Ty>
struct IteratorDerefEuqalTo
{
	typedef bool result_type;
	typedef Ty first_argument_type;
	typedef Ty second_argument_type;
	result_type operator()(const first_argument_type &it1,
		const second_argument_type &it2) const
	{
		return *it1==*it2;
	}
};



//×Ö·ûÊý×ÖÅÐ¶Ï
inline constexpr bool IsNumChar(char ch)
{
	return ch>='0' && ch<='9';
}
inline constexpr bool IsNotNumChar(char ch)
{
	return !IsNumChar(ch);
}
//×Ö·û´óÐ´×ÖÄ¸ÅÐ¶Ï
inline constexpr bool IsUppChar(char ch)
{
	return (ch>='A' && ch<='Z');
}
inline constexpr bool IsNotUppChar(char ch)
{
	return !IsUppChar(ch);
}
//×Ö·û´óÐ´×ÖÄ¸ÅÐ¶Ï
inline constexpr bool IsLowChar(char ch)
{
	return (ch>='a' && ch<='z');
}
inline constexpr bool IsNotLowChar(char ch)
{
	return !IsLowChar(ch);
}
//×Ö·û×ÖÄ¸ÅÐ¶Ï
inline constexpr bool IsLetChar(char ch)
{
	return IsUppChar(ch) || IsLowChar(ch);
}
inline constexpr bool IsNotLetChar(char ch)
{
	return !IsLetChar(ch);
}
//×Ö·ûÊý×Ö×ÖÄ¸ÅÐ¶Ï
inline constexpr bool IsNumLetChar(char ch)
{
	return IsNumChar(ch) || IsLetChar(ch);
}
inline constexpr bool IsNotNumLetChar(char ch)
{
	return !IsNumLetChar(ch);
}
//×Ö·û±êÊ¶·ûÅÐ¶Ï
inline constexpr bool IsIdChar(char ch)
{
	return IsNumLetChar(ch) || (ch=='_');
}
inline constexpr bool IsNotIdChar(char ch)
{
	return !IsIdChar(ch);
}
//×Ö·ûÊ®Áù½øÖÆÅÐ¶Ï
inline constexpr bool IsHexChar(char ch)
{
	return (ch>='0' && ch<='9')
		|| (ch>='a' && ch<='f') || (ch>='A' && ch<='F');
}
inline constexpr bool IsNotHexChar(char ch)
{
	return !IsHexChar(ch);
}
//×Ö·ûasciiÅÐ¶Ï
inline constexpr bool IsAsciiChar(char ch)
{
	return ch>=0x00 && ch<=0x7F;
}
inline constexpr bool IsNotAsciiChar(char ch)
{
	return !IsAsciiChar(ch);
}
//×Ö·û¹ãÒå±êÊ¶·ûÅÐ¶Ï
inline constexpr bool IsBroadIdChar(char ch)
{
	return IsIdChar(ch) || IsNotAsciiChar(ch);
}
inline constexpr bool IsNotBroadIdChar(char ch)
{
	return !IsBroadIdChar(ch);
}
//×Ö·û¿Õ°×·ûÅÐ¶Ï
inline constexpr bool IsBlankChar(char ch)
{
	return ch==' ' || ch=='\t' || ch=='\r' || ch=='\n'
		|| ch=='\v' || ch=='\f';
}
inline constexpr bool IsNotBlankChar(char ch)
{
	return !IsBlankChar(ch);
}


//Ð¡Ð´×ÖÄ¸×ª»»´óÐ´×ÖÄ¸
inline constexpr char LowCharToUppChar(char ch)
{
	return (char)
		(IsLowChar(ch) ?
		'A'+(ch-'a')
		:
		ch);
}
//´óÐ´×ÖÄ¸×ªÐ¡Ð´×ÖÄ¸
inline constexpr char UppCharToLowChar(char ch)
{
	return (char)
		(IsUppChar(ch) ?
		'a'+(ch-'A')
		:
		ch);
}


//Êý×Ö×Ö·û×ª»¯Êý×Ö
inline constexpr int NumCharToNum(char ch)
{
	return IsNumChar(ch) ?
		ch-'0'
		:
		-1;
}
//×ÖÄ¸×Ö·û×ª»¯Êý×Ö
inline constexpr int LetCharToNum(char ch)
{
	return IsLowChar(ch) ?
		ch-'a'
		: IsUppChar(ch) ?
		ch-'A'
		:
		-1;
}
//Ê®Áù½øÖÆ×Ö·û×ª»¯Êý×Ö
inline constexpr int HexCharToNum(char ch)
{
	return (ch>='0' && ch<='9') ?
		ch-'0'
		: (ch>='a' && ch<='f') ?
		ch-'a'+10
		: (ch>='A' && ch<='F') ?
		ch-'A'+10
		:
		-1;
}


//Êý×Ö×ª»¯Êý×Ö×Ö·û
inline constexpr char NumToNumChar(int i)
{
	return (char)
		((i>=0 && i<=9) ?
		i+'0'
		:
		0);
}
//Êý×Ö×ª»¯´óÐ´×ÖÄ¸×Ö·û
inline constexpr char NumToUppChar(int i)
{
	return (char)
		((i>=0 && i<=25) ?
		i+'A'
		:
		0);
}
//Êý×Ö×ª»¯Ð¡Ð´×ÖÄ¸×Ö·û
inline constexpr char NumToLowChar(int i)
{
	return (char)
		((i>=0 && i<=25) ?
		i+'a'
		:
		0);
}
//Êý×Ö×ª»¯´óÐ´Ê®Áù½øÖÆ×Ö·û
inline constexpr char NumToUppHexChar(int i)
{
	return (char)
		((i>=0 && i<=9) ?
		i+'0'
		: (i>=10 && i<=15) ?
		i+'A'
		:
		0);
}
//Êý×Ö×ª»¯Ð¡Ð´Ê®Áù½øÖÆ×Ö·û
inline constexpr char NumToLowHexChar(int i)
{
	return (char)
		((i>=0 && i<=9) ?
		i+'0'
		: (i>=10 && i<=15) ?
		i+'a'
		:
		0);
}

