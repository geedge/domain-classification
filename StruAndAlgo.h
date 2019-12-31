#pragma once

//Êý¾Ý½á¹¹ºÍËã·¨À©³ä
//20191212-2054

#include <cstddef>
#include <cmath>
#include <cassert>

#include <type_traits>
#include <limits>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <tuple>
#include <functional>
#include <iterator>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include <stdexcept>
#include <random>
#include <ratio>


//¿â¹ØÁªÐèÇóÎÄ¼þ
#include "TypeExtend.h"



//ÆäËû¿âÎÄ¼þÉùÃ÷

//tupleÀàÐÍµ÷ÓÃº¯Êý·µ»ØÖµÀàÐÍ£¬Ê¹ÓÃ´øcvrefµÄÀàÐÍ´«Èë
template<typename TyFunc, typename TyTup>
struct TupleInvokeReturnType;

//tupleÀàÐÍµ÷ÓÃº¯Êý£¬±¾ÖÊÉÏÖ»ÒªÄÜ±»getº¯Êý²ð½â¼´¿É£¬ÈôÐèÓÒÖµÔò½«tupleÒÔÓÒÖµ´«Èë
template<typename TyFunc, typename TyTup>
inline typename TupleInvokeReturnType<TyFunc &&, TyTup &&
>::type TupleInvoke(TyFunc &&func, TyTup &&tup);

//Ð´ÀàÐÍ¶þ½øÖÆÎÄ¼þÀà
//²»Í¬»·¾³µÄÄÚÖÃÀàÐÍ´óÐ¡²»Ò»¶¨ÏàÍ¬£¬Òª±£Ö¤Ê¹ÓÃ´óÐ¡ÎÞ¹Ø±äÁ¿
class BinWriteFile;

//¶ÁÀàÐÍ¶þ½øÖÆÎÄ¼þÀà
//²»Í¬»·¾³µÄÄÚÖÃÀàÐÍ´óÐ¡²»Ò»¶¨ÏàÍ¬£¬Òª±£Ö¤Ê¹ÓÃ´óÐ¡ÎÞ¹Ø±äÁ¿
class BinReadFile;




//¿ìËÙ¼ÆËãÃÝ´Î£¬×¢Òâ·µ»ØÀàÐÍºÍ»ùÀàÐÍÏàÍ¬
template<typename Ty>
inline constexpr Ty FastPower(Ty base, int power)
{
	return power<0 ? 0
		: power==0 ? 1
		: (power&1)==0 ? FastPower(base*base, power>>1)
		: base*FastPower(base*base, power>>1);
}



//ÍÆËã±ÈÀý±íÊ¾¾«¶È£¬¼´±ÈÀýÀ©´óµ×ÊýµÄ¼¸´ÎÃÝ¿ÉÒÔ´óÓÚµÈÓÚ»ù×¼±ÈÀý
template<typename Ratio, typename BaseRatio,
	typename CompareRatio =std::ratio<1>
> inline constexpr typename std::enable_if<
	std::ratio_greater_equal<Ratio, CompareRatio>::value, int
>::type RatioPrecision()
{
	return 0;
}
template<typename Ratio, typename BaseRatio,
	typename CompareRatio =std::ratio<1>
> inline constexpr typename std::enable_if<
	!std::ratio_greater_equal<Ratio, CompareRatio>::value, int
>::type RatioPrecision()
{
	return RatioPrecision<std::ratio_multiply<Ratio, BaseRatio>,
		BaseRatio, CompareRatio>()+1;
}



//Éú³ÉºóÒ»¸ö×éºÏÊý£¬·µ»ØÊÇ·ñµ½Í·
//ÒªÇóË«Ïòµü´úÆ÷£¬¿ÉµÝÔö¡¢ÏàµÈ±È½ÏÖµÀàÐÍ
template<typename TyIt, typename Ty>
bool NextCombination(TyIt st, TyIt ed, Ty rangeSt, Ty rangeEd)
{
	auto it= ed, it1= it;
	//ÕÒÊýÖµ¿ÕÏ¶
	for(; it!=st; --it) {
		it1 = it;
		-- it1;
		if(it==ed) {
			if(!(*it1==rangeEd)) {
				++ *it1;
				break;
			}
		}
		else {
			auto value = *it1;
			++ value;
			if(!(value==*it)) {
				*it1 = value;
				break;
			}
		}
	}
	//ºóÐø×éºÏÊýµÝÔö
	bool res = it!=st;
	if(it!=ed) {
		if(!res)
			*it = rangeSt;
		else {
			it1 = it;
			-- it1;
			*it = *it1;
			++ *it;
		}
		for(it1=it, ++it; it!=ed; it1=it, ++it) {
			*it = *it1;
			++ *it;
		}
	}
	return res;
}

//Éú³ÉºóÒ»¸öÅÅÁÐ×éºÏÊý£¬·µ»ØÊÇ·ñµ½Í·
//ÒªÇóË«Ïòµü´úÆ÷£¬¿ÉµÝÔö¡¢ÏàµÈ±È½ÏÖµÀàÐÍ
template<typename TyIt, typename Ty>
bool NextCombinationPermutation(TyIt st, TyIt ed, Ty rangeSt, Ty rangeEd)
{
	if(!std::next_permutation(st, ed)) {
		if(!NextCombination(st, ed, rangeSt, rangeEd))
			return false;
	}
	return true;
}



//¼ÆÊý¼¯ºÏ½»¼¯£¬ÏßÐÔ¸´ÔÓ¶È£¬ÒªÇóÒÑÅÅÐò
template<typename TyIt1, typename TyIt2,
	typename TyLess= GeneralLess<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>,
	typename TyEqual= GeneralEqualTo<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>
> inline size_t SetIntersectionCount(TyIt1 st1, TyIt1 ed1, TyIt2 st2, TyIt2 ed2,
	TyLess funcLess= TyLess(), TyEqual funcEqual= TyEqual())
{
	size_t ret = 0;
	while(true) {
		if(st1==ed1 || st2==ed2)
			break;
		//Á½Õß¶¼ÓÐ
		if(funcEqual(*st1, *st2)) {
			++st1, ++st2;
			++ ret;
		}
		//Ç°ÕßÓÐ£¬ºóÕßÃ»ÓÐ
		else if(funcLess(*st1, *st2)) {
			++ st1;
		}
		//ºóÕßÓÐ£¬Ç°ÕßÃ»ÓÐ
		else {
			++ st2;
		}
	}
	return ret;
}

//¼ÆÊý¼¯ºÏ²¢¼¯£¬ÏßÐÔ¸´ÔÓ¶È£¬ÒªÇóÒÑÅÅÐò
template<typename TyIt1, typename TyIt2,
	typename TyLess= GeneralLess<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>,
	typename TyEqual= GeneralEqualTo<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>
> inline size_t SetUnionCount(TyIt1 st1, TyIt1 ed1, TyIt2 st2, TyIt2 ed2,
	TyLess funcLess= TyLess(), TyEqual funcEqual= TyEqual())
{
	size_t ret = 0;
	while(true) {
		if(st1==ed1 || st2==ed2) {
			//ºóÕßÓÐÊ£Óà
			if(st1==ed1)
				ret += std::distance(st2, ed2);
			//Ç°ÕßÓÐÊ£Óà
			else
				ret += std::distance(st1, ed1);
			break;
		}
		//Á½Õß¶¼ÓÐ
		if(funcEqual(*st1, *st2)) {
			++st1, ++st2;
			++ ret;
		}
		//Ç°ÕßÓÐ£¬ºóÕßÃ»ÓÐ
		else if(funcLess(*st1, *st2)) {
			++ st1;
			++ ret;
		}
		//ºóÕßÓÐ£¬Ç°ÕßÃ»ÓÐ
		else {
			++ st2;
			++ ret;
		}
	}
	return ret;
}

//¼ÆÊý¼¯ºÏ²î¼¯£¬ÏßÐÔ¸´ÔÓ¶È£¬ÒªÇóÒÑÅÅÐò
template<typename TyIt1, typename TyIt2,
	typename TyLess= GeneralLess<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>,
	typename TyEqual= GeneralEqualTo<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>
> inline size_t SetDifferenceCount(TyIt1 st1, TyIt1 ed1, TyIt2 st2, TyIt2 ed2,
	TyLess funcLess= TyLess(), TyEqual funcEqual= TyEqual())
{
	size_t ret = 0;
	while(true) {
		if(st1==ed1 || st2==ed2) {
			//ºóÕßÓÐÊ£Óà
			if(st1==ed1)
				;
			//Ç°ÕßÓÐÊ£Óà
			else
				ret += std::distance(st1, ed1);
			break;
		}
		//Á½Õß¶¼ÓÐ
		if(funcEqual(*st1, *st2)) {
			++st1, ++st2;
		}
		//Ç°ÕßÓÐ£¬ºóÕßÃ»ÓÐ
		else if(funcLess(*st1, *st2)) {
			++ st1;
			++ ret;
		}
		//ºóÕßÓÐ£¬Ç°ÕßÃ»ÓÐ
		else {
			++ st2;
		}
	}
	return ret;
}

//¼ÆÊý¼¯ºÏ¶Ô³Æ²î¼¯£¬ÏßÐÔ¸´ÔÓ¶È£¬ÒªÇóÒÑÅÅÐò
template<typename TyIt1, typename TyIt2,
	typename TyLess= GeneralLess<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>,
	typename TyEqual= GeneralEqualTo<typename IteratorRemoveCVRefDerefType<TyIt1>::type,
	typename IteratorRemoveCVRefDerefType<TyIt2>::type>
> inline size_t SetSymmetricDifferenceCount(TyIt1 st1, TyIt1 ed1, TyIt2 st2, TyIt2 ed2,
	TyLess funcLess= TyLess(), TyEqual funcEqual= TyEqual())
{
	size_t ret = 0;
	while(true) {
		if(st1==ed1 || st2==ed2) {
			//ºóÕßÓÐÊ£Óà
			if(st1==ed1)
				ret += std::distance(st2, ed2);
			//Ç°ÕßÓÐÊ£Óà
			else
				ret += std::distance(st1, ed1);
			break;
		}
		//Á½Õß¶¼ÓÐ
		if(funcEqual(*st1, *st2)) {
			++st1, ++st2;
		}
		//Ç°ÕßÓÐ£¬ºóÕßÃ»ÓÐ
		else if(funcLess(*st1, *st2)) {
			++ st1;
			++ ret;
		}
		//ºóÕßÓÐ£¬Ç°ÕßÃ»ÓÐ
		else {
			++ st2;
			++ ret;
		}
	}
	return ret;
}




//²¢²é¼¯£¬Êý×ÖË÷Òý£¬Êý×é½á¹¹£¬¿ÉÒÔÎª¿ÕÀàÐÍ
template<typename TyData= void>
class IndexDisjointSet
{
private:
	//²¢²é¼¯½ÚµãÊý¾Ý½á¹¹
	struct Node:
		public ValueHolder<TyData>
	{
		size_t idxPr;//¸¸Ö¸Õë
		size_t rank = 0;//ÖÈ£¬Ëã·¨ÓÃ
		template<typename ...Ty_S>
		Node(size_t idx, Ty_S &&...arg_s):
			ValueHolder<TyData>(std::forward<Ty_S>(arg_s)...), idxPr(idx)
		{
		}
	};

private:
	std::vector<Node> vec;//Êý¾Ý

public:
	//¹¹Ôìº¯Êý
	IndexDisjointSet()
		= default;
	IndexDisjointSet(const IndexDisjointSet &)
		= default;
	IndexDisjointSet &operator =(const IndexDisjointSet &)
		= default;
	IndexDisjointSet(IndexDisjointSet &&)
		= default;
	IndexDisjointSet &operator =(IndexDisjointSet &&)
		= default;
	//¶àÊý¾Ý¹¹Ôì
	template<typename ...Ty_S>
	explicit IndexDisjointSet(size_t size, Ty_S &&...arg_s)
	{
		Assign(size, std::forward<Ty_S>(arg_s)...);
	}
public:
	//¶àÊý¾Ý¸³Öµ
	template<typename ...Ty_S>
	IndexDisjointSet &Assign(size_t size, Ty_S &&...arg_s)
	{
		for(size_t i=0; i!=size; ++i)
			Emplace(std::forward<Ty_S>(arg_s)...);
		return *this;
	}
	//»ñÈ¡Êý¾Ý
	typename Node::RefType operator[](size_t idx)
	{
		return vec[idx];
	}
	typename Node::ConstRefType operator[](size_t idx) const
	{
		return vec[idx];
	}
	//»ñÈ¡´óÐ¡
	size_t Size() const
	{
		return vec.size();
	}
	//Çå¿Õ
	void Clear()
	{
		vec.clear();
	}
	//Ìí¼Óµã£¬·µ»Ø¸ù
	template<typename ...Ty_S>
	size_t Emplace(Ty_S &&...arg_s)
	{
		size_t root = vec.size();
		vec.emplace_back(root, std::forward<Ty_S>(arg_s)...);
		return root;
	}
	//Ìí¼ÓµãÍ¬Ê±Á¬½Ó£¬·µ»Ø¸ù
	template<typename ...Ty_S>
	size_t EmplaceUnion(size_t idx, Ty_S &&...arg_s)
	{
		size_t root = vec.size();
		vec.emplace_back(root, std::forward<Ty_S>(arg_s)...);
		return Union(root, idx);
	}
	//ÕÒ¸ù½Úµã
	size_t FindRoot(size_t idx)
	{
		if(vec[idx].idxPr!=idx)
			//ÕÒµÄÍ¬Ê±Á¬½Óµ½¸ù
			vec[idx].idxPr = FindRoot(vec[idx].idxPr);
		return vec[idx].idxPr;
	}
	//ºÏ²¢¼¯ºÏ£¬·µ»Ø¸ù
	size_t Union(size_t idx1, size_t idx2)
	{
		size_t root1 = FindRoot(idx1), root2 = FindRoot(idx2);
		if(vec[root1].rank<=vec[root2].rank) {
			//1Á¬µ½2ÉÏ
			vec[root1].idxPr = root2;
			if(vec[root1].rank==vec[root2].rank)
				++ vec[root2].rank;
			return root2;
		}
		else {
			//2Á¬µ½1ÉÏ
			vec[root2].idxPr = root1;
			return root1;
		}
	}
	//ÈÝÆ÷½Ó¿Ú
	void Reserve(size_t size)
	{
		vec.reserve(size);
	}
	size_t Capacity() const
	{
		return vec.capacity();
	}
	void ShinkToFit()
	{
		return vec.shink_to_fit();
	}
};


template<typename TyKey, typename TyValue= void, typename TyCom= std::less<TyKey>>
class MapDisjointSetNode;
//Ê÷ÐÎ²¢²é¼¯ÀàÐÍ¼òÐ´£¬Ö§³ÖÁ¬½ÓºÍÌí¼Ó»ìÓÃ£¬²¢²é¼¯±êÇ©ÎªÊ÷µü´úÆ÷
template<typename TyKey, typename TyValue= void, typename TyCom= std::less<TyKey>>
using MapDisjointSet = std::map<TyKey, MapDisjointSetNode<TyKey, TyValue, TyCom>, TyCom>;
//Ê÷ÐÎ²¢²é¼¯£¬½Úµã½á¹¹£¬½«Æä¸³¸ømapÄ£°åÀàÐÍÊ¹ÓÃ
template<typename TyKey, typename TyValue, typename TyCom>
class MapDisjointSetNode:
	private ValueHolder<TyValue>
{
	template<typename TyIt>
	friend TyIt MapDisjointSetFindRoot(TyIt it);
	template<typename TyIt>
	friend TyIt MapDisjointSetUnion(TyIt it1, TyIt it2);
private:
	typename MapDisjointSet<TyKey, TyValue, TyCom>::iterator itPr;//´æ´¢Ö¸Õë
	size_t rank = 0;//ÖÈ£¬Ëã·¨ÓÃ
	bool bRoot = true;//¼ÇÂ¼ÊÇ·ñÎª¸ù£¬Îª¸ùÊ±²»Ðè±£Ö¤Ö¸ÕëÓÐÐ§
public:
	//¹¹Ôìº¯Êý
	template<typename ...Ty_S>
	MapDisjointSetNode(Ty_S &&...arg_s):
		ValueHolder<TyValue>(std::forward<Ty_S>(arg_s)...)
	{
	}
	//ÒýÓÃÔ­Ê¼Êý¾Ý
	typename ValueHolder<TyValue>::RefType Get()
	{
		return this->hold;
	}
	typename ValueHolder<TyValue>::ConstRefType Get() const
	{
		return this->hold;
	}
};
//Ê÷ÐÎ²¢²é¼¯¾²Ì¬Ëã·¨£¬ÕÒ¸ù½Úµã£¬½ÓÊÜÖ¸Õë»òµü´úÆ÷
template<typename TyIt>
inline TyIt MapDisjointSetFindRoot(TyIt it)
{
	if(it->second.bRoot)
		return it;
	else {
		it->second.itPr = MapDisjointSetFindRoot(it->second.itPr);
		return it->second.itPr;
	}
}
//Ê÷ÐÎ²¢²é¼¯¾²Ì¬Ëã·¨£¬ºÏ²¢¼¯ºÏ£¬½ÓÊÜÖ¸Õë»òµü´úÆ÷£¬·µ»Ø¸ù
template<typename TyIt>
inline TyIt MapDisjointSetUnion(TyIt it1, TyIt it2)
{
	TyIt root1= MapDisjointSetFindRoot(it1), root2= MapDisjointSetFindRoot(it2);
	if(root1==root2)
		return root1;
	if(root1->second.rank<=root2->second.rank) {
		//1Á¬µ½2ÉÏ
		root1->second.itPr = root2;
		root1->second.bRoot = false;
		if(root1->second.rank==root2->second.rank)
			++ root2->second.rank;
		return root2;
	}
	else {
		//2Á¬µ½1ÉÏ
		root2->second.itPr = root1;
		root2->second.bRoot = false;
		return root1;
	}
}


template<typename TyKey, typename TyValue= void,
	typename TyHash= std::hash<TyKey>, typename TyEqual= std::equal_to<TyKey>
> class UnordMapDisjointSetNode;
//¹þÏ£²¢²é¼¯ÀàÐÍ¼òÐ´£¬Ö§³ÖÁ¬½ÓºÍÌí¼Ó»ìÓÃ£¬²¢²é¼¯±êÇ©ÎªÄÚÈÝpairÖ¸Õë
template<typename TyKey, typename TyValue= void,
	typename TyHash= std::hash<TyKey>, typename TyEqual= std::equal_to<TyKey>
> using UnordMapDisjointSet = std::unordered_map<TyKey,
	UnordMapDisjointSetNode<TyKey, TyValue, TyHash, TyEqual>, TyHash, TyEqual>;
//¹þÏ£²¢²é¼¯£¬½Úµã½á¹¹£¬½«Æä¸³¸øunordered_mapÄ£°åÀàÐÍÊ¹ÓÃ
template<typename TyKey, typename TyValue, typename TyHash, typename TyEqual>
class UnordMapDisjointSetNode:
	private ValueHolder<TyValue>
{
	template<typename TyIt>
	friend auto UnordMapDisjointSetFindRoot(TyIt it)-> decltype(&*it);
	template<typename TyIt>
	friend auto UnordMapDisjointSetUnion(TyIt it1, TyIt it2)-> decltype(&*it1);
private:
	typename UnordMapDisjointSet<TyKey, TyValue, TyHash, TyEqual>::pointer pPr= nullptr;//´æ´¢Ö¸Õë
	size_t rank = 0;//ÖÈ£¬Ëã·¨ÓÃ
	bool bRoot = true;//¼ÇÂ¼ÊÇ·ñÎª¸ù£¬Îª¸ùÊ±²»Ðè±£Ö¤Ö¸ÕëÓÐÐ§
public:
	//¹¹Ôìº¯Êý
	template<typename ...Ty_S>
	UnordMapDisjointSetNode(Ty_S &&...arg_s):
		ValueHolder<TyValue>(std::forward<Ty_S>(arg_s)...)
	{
	}
	//ÒýÓÃÔ­Ê¼Êý¾Ý
	typename ValueHolder<TyValue>::RefType Get()
	{
		return this->hold;
	}
	typename ValueHolder<TyValue>::ConstRefType Get() const
	{
		return this->hold;
	}
};
//¹þÏ£²¢²é¼¯¾²Ì¬Ëã·¨£¬ÕÒ¸ù½Úµã£¬½ÓÊÜÖ¸Õë»òµü´úÆ÷
template<typename TyIt>
inline auto UnordMapDisjointSetFindRoot(TyIt it)-> decltype(&*it)
{
	if(it->second.bRoot)
		return &*it;
	else {
		auto itRes = UnordMapDisjointSetFindRoot(it->second.pPr);
		it->second.pPr = &*itRes;
		return it->second.pPr;
	}
}
//¹þÏ£²¢²é¼¯¾²Ì¬Ëã·¨£¬ºÏ²¢¼¯ºÏ£¬½ÓÊÜÖ¸Õë»òµü´úÆ÷£¬·µ»Ø¸ù
template<typename TyIt>
inline auto UnordMapDisjointSetUnion(TyIt it1, TyIt it2)-> decltype(&*it1)
{
	auto root1= UnordMapDisjointSetFindRoot(it1),
		root2= UnordMapDisjointSetFindRoot(it2);
	if(root1==root2)
		return root1;
	if(root1->second.rank<=root2->second.rank) {
		//1Á¬µ½2ÉÏ
		root1->second.pPr = root2;
		root1->second.bRoot = false;
		if(root1->second.rank==root2->second.rank)
			++ root2->second.rank;
		return root2;
	}
	else {
		//2Á¬µ½1ÉÏ
		root2->second.pPr = root1;
		root2->second.bRoot = false;
		return root1;
	}
}



//Æ¥ÅäÊ÷ÄÚ²¿½ÚµãÀàÐÍ£¬Æ½ºâÊ÷ÊµÏÖ
template<typename TyMatch, typename TyData>
struct _TrieTreeNode
{
	using RetDataType = TyData *;
	using ConstRetDataType = const TyData *;
	constexpr static bool bIsVoid = false;
public:
	TyData *pData = nullptr;
	std::map<TyMatch, _TrieTreeNode<TyMatch, TyData>> mapCh;//×ÓÊ÷
public:
	_TrieTreeNode()
		= default;
	~_TrieTreeNode()
	{
		delete pData;
	}
	//¿½±´º¯Êý
	_TrieTreeNode(const _TrieTreeNode &other)
	{
		operator =(other);
	}
	_TrieTreeNode &operator =(const _TrieTreeNode &other)
	{
		if(this!=&other) {
			//Èô¶Ô·½ÓÐÊý¾Ý
			if(other.pData) {
				if(pData)
					*pData = *other.pData;
				else
					Construct(other.data);
			}
			//Èô¶Ô·½Ã»Êý¾Ý
			else {
				if(pData) {
					delete pData;
					pData = nullptr;
				}
			}
			//¿½±´ÆäËûÊý¾Ý
			mapCh = other.mapCh;
		}
		return *this;
	}
	//¹¹ÔìÊý¾Ý
	template<typename ...Ty_S>
	void Construct(Ty_S &&...arg_s)
	{
		assert(pData==nullptr);
		pData = new TyData(std::forward<Ty_S>(arg_s)...);
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		delete pData;
		pData = nullptr;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾ÝÖ¸Õë
	static RetDataType GetData(_TrieTreeNode *pt)
	{
		if(pt==nullptr)
			return nullptr;
		else
			return pt->pData;
	}
	static ConstRetDataType GetData(const _TrieTreeNode *pt)
	{
		return GetData((_TrieTreeNode *)pt);
	}
};
template<typename TyMatch>
struct _TrieTreeNode<TyMatch, void>
{
	using RetDataType = bool;
	using ConstRetDataType = bool;
	constexpr static bool bIsVoid = true;
public:
	bool bData = false;//ÊÇ·ñÓÐÊý¾Ý
	std::map<TyMatch, _TrieTreeNode<TyMatch, void>> mapCh;//×ÓÊ÷
public:
	//¹¹ÔìÊý¾Ý
	void Construct()
	{
		bData = true;
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		bData = false;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾Ý±êÇ©
	static ConstRetDataType GetData(const _TrieTreeNode *pt)
	{
		if(pt==nullptr)
			return false;
		else
			return pt->bData;
	}
};

//Æ¥ÅäÊ÷ÄÚ²¿½ÚµãÀàÐÍ£¬¹þÏ£±íÊµÏÖ
template<typename TyMatch, typename TyData>
struct _UnordTrieTreeNode
{
	using RetDataType = TyData *;
	using ConstRetDataType = const TyData *;
	constexpr static bool bIsVoid = false;
public:
	TyData *pData = nullptr;
	std::unordered_map<TyMatch, _UnordTrieTreeNode<TyMatch, TyData>> mapCh;//×ÓÊ÷
public:
	_UnordTrieTreeNode()
		= default;
	~_UnordTrieTreeNode()
	{
		delete pData;
	}
	//¿½±´º¯Êý
	_UnordTrieTreeNode(const _UnordTrieTreeNode &other)
	{
		operator =(other);
	}
	_UnordTrieTreeNode &operator =(const _UnordTrieTreeNode &other)
	{
		if(this!=&other) {
			//Èô¶Ô·½ÓÐÊý¾Ý
			if(other.pData) {
				if(pData)
					*pData = *other.pData;
				else
					Construct(other.data);
			}
			//Èô¶Ô·½Ã»Êý¾Ý
			else {
				if(pData) {
					delete pData;
					pData = nullptr;
				}
			}
			//¿½±´ÆäËûÊý¾Ý
			mapCh = other.mapCh;
		}
		return *this;
	}
	//¹¹ÔìÊý¾Ý
	template<typename ...Ty_S>
	void Construct(Ty_S &&...arg_s)
	{
		assert(pData==nullptr);
		pData = new TyData(std::forward<Ty_S>(arg_s)...);
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		delete pData;
		pData = nullptr;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾ÝÖ¸Õë
	static RetDataType GetData(_UnordTrieTreeNode *pt)
	{
		if(pt==nullptr)
			return nullptr;
		else
			return pt->pData;
	}
	static ConstRetDataType GetData(const _UnordTrieTreeNode *pt)
	{
		return GetData((_UnordTrieTreeNode *)pt);
	}
};
template<typename TyMatch>
struct _UnordTrieTreeNode<TyMatch, void>
{
	using RetDataType = bool;
	using ConstRetDataType = bool;
	constexpr static bool bIsVoid = true;
public:
	bool bData = false;//ÊÇ·ñÓÐÊý¾Ý
	std::unordered_map<TyMatch, _UnordTrieTreeNode<TyMatch, void>> mapCh;//×ÓÊ÷
public:
	//¹¹ÔìÊý¾Ý
	void Construct()
	{
		bData = true;
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		bData = false;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾Ý±êÇ©
	static ConstRetDataType GetData(const _UnordTrieTreeNode *pt)
	{
		if(pt==nullptr)
			return false;
		else
			return pt->bData;
	}
};

//binfile½Ó¿Ú¸¨Öú£¬¿ÉÒÔ¼æÈÝºóÃæµÄAcMachineNode
template<typename TyNode>
inline typename std::enable_if<!TyNode::bIsVoid, void
>::type _BinWriteTrieOrAcNodeAssist(BinWriteFile &bwf, TyNode &node)
{
	uint8_t bData = node.pData!=nullptr;
	bwf <<bData;
	if(bData)
		bwf <<*node.pData;
	bwf <<node.mapCh;
}
template<typename TyNode>
inline typename std::enable_if<TyNode::bIsVoid, void
>::type _BinWriteTrieOrAcNodeAssist(BinWriteFile &bwf, TyNode &node)
{
	bwf <<(uint8_t)node.bData;
	bwf <<node.mapCh;
}
template<typename TyNode>
inline typename std::enable_if<!TyNode::bIsVoid, void
>::type _BinReadTrieOrAcNodeAssist(BinReadFile &brf, TyNode &node)
{
	uint8_t bData;
	brf >>bData;
	//Èô¶Ô·½ÓÐÊý¾Ý
	if(bData) {
		if(node.pData==nullptr)
			node.Construct();
		brf >>*node.pData;
	}
	//Èô¶Ô·½Ã»Êý¾Ý
	else {
		if(node.pData) {
			delete node.pData;
			node.pData = nullptr;
		}
	}
	brf >>node.mapCh;
}
template<typename TyNode>
inline typename std::enable_if<TyNode::bIsVoid, void
>::type _BinReadTrieOrAcNodeAssist(BinReadFile &brf, TyNode &node)
{
	uint8_t bData;
	brf >>bData;
	node.bData = bData;
	brf >>node.mapCh;
}

//binfile½Ó¿Ú
template<typename TyMatch, typename TyData>
inline BinWriteFile &operator <<(BinWriteFile &bwf, const _TrieTreeNode<TyMatch, TyData> &node)
{
	_BinWriteTrieOrAcNodeAssist(bwf, node);
	return bwf;
}
template<typename TyMatch, typename TyData>
inline BinReadFile &operator >>(BinReadFile &brf, _TrieTreeNode<TyMatch, TyData> &node)
{
	_BinReadTrieOrAcNodeAssist(brf, node);
	return brf;
}
template<typename TyMatch, typename TyData>
inline BinWriteFile &operator <<(BinWriteFile &bwf, const _UnordTrieTreeNode<TyMatch, TyData> &node)
{
	_BinWriteTrieOrAcNodeAssist(bwf, node);
	return bwf;
}
template<typename TyMatch, typename TyData>
inline BinReadFile &operator >>(BinReadFile &brf, _UnordTrieTreeNode<TyMatch, TyData> &node)
{
	_BinReadTrieOrAcNodeAssist(brf, node);
	return brf;
}

template<typename TyNode>
class _TrieTreeTemplate;
//Ê÷ÐÎÆ¥Åä½á¹¹ÉÏÏÂÎÄ
template<typename TyNode, typename TyIt>
struct _TrieContextTemplate
{
	bool bFirst = true;//¼ÇÂ¼Ê×´ÎÆ¥Åä
	TyNode *pt;//Ê÷½ÚµãÖ¸Õë
	TyIt it;//Æ¥Åä´®Ö¸Õë
};
template<typename TyNode, typename TyIt>
inline _TrieContextTemplate<TyNode, TyIt> MakeTrieContext(const _TrieTreeTemplate<TyNode> &, TyIt)
{
	return _TrieContextTemplate<TyNode, TyIt>();
}

//Ê÷×´´®Æ¥Åä½á¹¹£¬Ö§³ÖvoidÊý¾Ý
template<typename TyNode>
class _TrieTreeTemplate
{
	template<typename TyNode2>
	friend BinWriteFile &operator <<(BinWriteFile &bwf, const _TrieTreeTemplate<TyNode2> &trie);
	template<typename TyNode2>
	friend BinReadFile &operator >>(BinReadFile &brf, _TrieTreeTemplate<TyNode2> &trie);

public:
	//¸¨ÖúÀàÐÍ
	using NodeType = TyNode;
	using RetDataType = typename NodeType::RetDataType;
	using ConstRetDataType = typename NodeType::ConstRetDataType;

private:
	NodeType *pRoot;//Ê÷¸ù½Úµã

public:
	//Îå¿½±´
	_TrieTreeTemplate():
		pRoot(new NodeType)
	{
	}
	~_TrieTreeTemplate()
	{
		delete pRoot;
	}
	_TrieTreeTemplate(const _TrieTreeTemplate &other):
		_TrieTreeTemplate()
	{
		operator =(other);
	}
	_TrieTreeTemplate(_TrieTreeTemplate &&other):
		_TrieTreeTemplate()
	{
		operator =(std::move(other));
	}
	_TrieTreeTemplate &operator =(const _TrieTreeTemplate &other)
	{
		if(this!=&other) {
			*pRoot = *other.pRoot;
		}
		return *this;
	}
	_TrieTreeTemplate &operator =(_TrieTreeTemplate &&other)
	{
		if(this!=&other) {
			std::swap(pRoot, other.pRoot);
		}
		return *this;
	}
private:
	//ÕÒ½Úµã¸¨Öúº¯Êý£¬¿Õ´®¿ÉÒÔÆ¥Åä¿ÕÆ¥Åä£¬pNode·µ»ØÆ¥ÅäµÄ½Úµã»ò¿Õ
	//itSt·µ»ØÆ¥ÅäµÄÎ²ºó£¬bShortÕÒµ½¼´½áÊø£¬bBlankÔÊÐí¿ÕÆ¥Åä£¬bAdd²éÕÒÊ±ÏòºóÌí¼Ó
	template<typename TyIt>
	static void FindAssist(NodeType *&pNode, TyIt &itSt, TyIt itEd,
		bool bShort, bool bBlank, bool bAdd)
	{
		//ÅÐ¶Ï¿ÕÖ¸Õë
		if(pNode==nullptr)
			return;
		//ÔÊÐíÎª¿ÕÊ±µÄ´¦Àí
		if(bBlank) {
			//ÌáÇ°½áÊøÇÒÆ¥Åä£¬»òÒÑ¾­Ã»ÓÐÆ¥ÅäºóÐø£¬Ôò·µ»Ø³õÊ¼½Úµã
			if((bShort && NodeType::GetData(pNode)) || itSt==itEd)
				return;
		}
		//ÈôÌáÇ°½áÊøÊ§°ÜÇÒÃ»ÓÐºóÐøÆ¥Åä£¬ÔòÈÏÎªÕÒ²»µ½
		if(itSt==itEd) {
			pNode = nullptr;
		}
		//Ö÷Ñ­»·
		while(itSt!=itEd) {//µü´úÆ÷½áÊøÔòÌø³ö£¬´ËÊ±·µ»ØÖµÎªÕÒµ½µÄ½Úµã
								 //¸ù¾ÝÌí¼ÓÇé¿öÏòºóµü´ú
			if(bAdd)
				pNode = &pNode->mapCh[*itSt];
			else {
				auto itPr = pNode->mapCh.find(*itSt);
				//²»Ìí¼ÓÇÒÕÒ²»µ½¾Í·µ»Ø¿Õ
				if(itPr==pNode->mapCh.end()) {
					pNode = nullptr;
					break;
				}
				pNode = &itPr->second;
			}
			++ itSt;
			//ÅÐÌáÇ°½áÊø£¬´ËÊ±µü´úÆ÷ÒÑ³ÉÎªÎ²ºó
			if(bShort && NodeType::GetData(pNode))
				break;
		}
	}
public:
	//Ìí¼ÓÆ¥Åä¹æÔò
	template<typename TyIt, typename ...Ty_S>
	bool EmplaceRule(TyIt itSt, TyIt itEd, Ty_S &&...arg_s)
	{
		NodeType *pNode = pRoot;
		FindAssist(pNode, itSt, itEd, false, true, true);
		if(NodeType::GetData(pNode))
			return false;
		pNode->Construct(std::forward<Ty_S>(arg_s)...);
		return true;
	}
	//Çå³ý½Úµã
	void Clear()
	{
		pRoot->Clear();
	}
	//Æ¥ÅäÍêÕû´®£¬Êý¾ÝÀàÐÍ·µ»ØÖ¸Õë£¬¿ÕÀàÐÍ·µ»Ø²¼¶û
	//ÕÒ²»µ½·µ»Ø¿ÕÖ¸Õë»ò·ñ£¬ÈôÕÒµ½Ôòµü´úÆ÷ÎªÎ²ºó£¬ÈôÎ´ÕÒµ½µü´úÆ÷´æÆ¥Åä²»µ½µÄµÚÒ»¸öÎ»ÖÃ
	template<typename TyIt>
	std::pair<TyIt, RetDataType> Match(TyIt itSt, TyIt itEd)
	{
		NodeType *pNode = pRoot;
		FindAssist(pNode, itSt, itEd, false, true, false);
		return std::make_pair(itSt, NodeType::GetData(pNode));
	}
	template<typename TyIt>
	std::pair<TyIt, ConstRetDataType> Match(TyIt itSt, TyIt itEd) const
	{
		return ((_TrieTreeTemplate *)this)->Match(itSt, itEd);
	}
	//µü´úËÑË÷´®£¬Êý¾ÝÀàÐÍ·µ»ØÖ¸Õë£¬¿ÕÀàÐÍ·µ»Ø²¼¶û
	//ÕÒ²»µ½·µ»Ø¿ÕÖ¸Õë£¬ÈôÕÒµ½Ôòµü´úÆ÷ÎªÎ²ºó£¬ÈôÎ´ÕÒµ½µü´úÆ÷´æÆ¥Åä²»µ½µÄµÚÒ»¸öÎ»ÖÃ
	//ctx³õÊ¼¸³Ä¬ÈÏ¹¹Ôì£¬Ö®ºóÁ¬ÐøÊ¹ÓÃ´Ë×÷ÎªÖÐ¼ä¼þ
	template<typename TyIt, typename TyCtx= _TrieContextTemplate<TyNode, TyIt>>
	typename std::enable_if<IsRemoveCVRefSame<_TrieContextTemplate<TyNode, TyIt>,
		TyCtx>::value, std::pair<TyIt, RetDataType>
	>::type Search(TyIt itSt, TyIt itEd, TyCtx &&ctx= TyCtx())
	{
		bool bBlank = false;
		if(ctx.bFirst) {
			ctx.bFirst = false;
			ctx.pt = pRoot;
			ctx.it = itSt;
			bBlank = true;//µÚÒ»´Î¿ÉÒÔÆ¥Åä¿Õ£¬Îª±£Ö¤ÍÆ½øºóÐø²»ÐÐ
		}
		FindAssist(ctx.pt, ctx.it, itEd, true, bBlank, false);
		return std::make_pair(ctx.it, NodeType::GetData(ctx.pt));
	}
	template<typename TyIt, typename TyCtx= _TrieContextTemplate<TyNode, TyIt>>
	typename std::enable_if<IsRemoveCVRefSame<_TrieContextTemplate<TyNode, TyIt>,
		TyCtx>::value, std::pair<TyIt, ConstRetDataType>
	>::type Search(TyIt itSt, TyIt itEd, TyCtx &&ctx= TyCtx()) const
	{
		return ((_TrieTreeTemplate *)this)->Search(itSt, itEd, ctx);
	}
	//Æ¥Åä×î³¤´®£¬Êý¾ÝÀàÐÍ·µ»ØÖ¸Õë£¬¿ÕÀàÐÍ·µ»Ø²¼¶û
	//ÕÒ²»µ½·µ»Ø¿ÕÖ¸Õë£¬ÈôÕÒµ½Ôòµü´úÆ÷ÎªÎ²ºó£¬ÈôÎ´ÕÒµ½µü´úÆ÷´æÆ¥Åä²»µ½µÄµÚÒ»¸öÎ»ÖÃ
	template<typename TyIt>
	std::pair<TyIt, RetDataType> SearchLong(TyIt itSt, TyIt itEd)
	{
		_TrieContextTemplate<TyNode, TyIt> ctx;
		//Ê×ÏÈ²éÕÒÒ»´Î
		auto ret = Search(itSt, itEd, ctx);
		//ÈôÕÒµÃµ½Ôò¼ÌÐøÕÒ
		if(ret.second) {
			while(true) {
				auto tmp = Search(itSt, itEd, ctx);
				if(!tmp.second)
					break;
				ret = tmp;
			}
		}
		return ret;
	}
	template<typename TyIt>
	std::pair<TyIt, ConstRetDataType> SearchLong(TyIt itSt, TyIt itEd) const
	{
		return ((_TrieTreeTemplate *)this)->SearchLong(itSt, itEd);
	}
	//ÕÒµ½µÚÒ»¸ö
	template<typename TyIt>
	RetDataType Judge(TyIt itSt, TyIt itEd)
	{
		return Search(itSt, itEd).second;
	}
	template<typename TyIt>
	ConstRetDataType Judge(TyIt itSt, TyIt itEd) const
	{
		return ((_TrieTreeTemplate *)this)->Judge(itSt, itEd);
	}
	//¼ÆÊýÕÒµ½´ÎÊý
	template<typename TyIt>
	size_t Count(TyIt itSt, TyIt itEd) const
	{
		_TrieContextTemplate<TyNode, TyIt> ctx;
		size_t cnt = 0;
		for(; ; ++cnt) {
			if(!Search(itSt, itEd, ctx).second)
				break;
		}
		return cnt;
	}
};

//binfile½Ó¿Ú
template<typename TyNode>
inline BinWriteFile &operator <<(BinWriteFile &bwf, const _TrieTreeTemplate<TyNode> &trie)
{
	return bwf <<*trie.pRoot;
}
template<typename TyNode>
inline BinReadFile &operator >>(BinReadFile &brf, _TrieTreeTemplate<TyNode> &trie)
{
	return brf >>*trie.pRoot;
}

//¶¨ÒåËÑË÷Ê÷µÄÆ½ºâÊ÷ÊµÏÖ
template<typename TyMatch, typename TyData= void>
using TrieTree = _TrieTreeTemplate<_TrieTreeNode<TyMatch, TyData>>;
template<typename TyMatch, typename TyData, typename TyIt>
using TrieContext = _TrieContextTemplate<_TrieTreeNode<TyMatch, TyData>, TyIt>;
//¶¨ÒåËÑË÷Ê÷µÄ¹þÏ£±íÊµÏÖ
template<typename TyMatch, typename TyData= void>
using UnordTrieTree = _TrieTreeTemplate<_UnordTrieTreeNode<TyMatch, TyData>>;
template<typename TyMatch, typename TyData, typename TyIt>
using UnordTrieContext = _TrieContextTemplate<_UnordTrieTreeNode<TyMatch, TyData>, TyIt>;



//KMPËÑË÷ÀàÉÏÏÂÎÄ
template<typename TyItT, typename TyItP>
struct KmpContext
{
	ptrdiff_t cnt = -1;//ÒÑÆ¥ÅäµÄ¸öÊý£¬¸ºÊý±íÊ¾Î´³õÊ¼»¯
	TyItP itSub;//Æ¥ÅäÄ£Ê½´®µÄµü´úÎ»ÖÃ
	TyItT it;//ÎÄ±¾´®Î»ÖÃ
	TyItT itTFlag;//ÎÄ±¾´®¶ÔÆë¿ªÊ¼Î»ÖÃ
};
template<typename TyItT, typename TyItP>
inline KmpContext<TyItT, TyItP> MakeKmpContext(TyItT, TyItP)
{
	return KmpContext<TyItT, TyItP>();
}

//KMPËÑË÷Àà
class KmpSearcher
{
	std::vector<ptrdiff_t> vecSub;//¼ÇÂ¼ÓÒÒÆµÄ³¤¶È£¬µÚiÏî¼ÇÂ¼µÄÊÇÒÑÆ¥Åäi+1¸öºóµÄÒÆ¶¯³¤¶È£¬¼´ÏÂ±êiÎ»ÖÃ
public:
	KmpSearcher()
		= default;
	template<typename TyIt>
	KmpSearcher(TyIt itSt, TyIt itEd)
	{
		Assign(itSt, itEd);
	}
public:
	//¸³Öµº¯Êý
	template<typename TyIt>
	KmpSearcher &Assign(TyIt itSt, TyIt itEd)
	{
		vecSub.resize(std::distance(itSt, itEd));
		if(vecSub.empty())
			return *this;
		vecSub[1-1] = 1;//ÈôÒÑÆ¥Åä1¸ö£¬ÔòÓ¦¸ÃÓÒÒÆ1
		ptrdiff_t cnt = 0;//ÒÑÆ¥ÅäµÄ¸öÊý
		auto itSub = itSt;//Æ¥ÅäÄ£Ê½´®µÄµü´úÎ»ÖÃ
		ptrdiff_t locate = 1;//µ±Ç°¶ÁÈ¡´®µÄÏÂ±ê
									//Ö÷ÌåÑ­»·
		for(++itSt; itSt!=itEd; ++itSt, ++locate) {
			while(cnt!=0 && *itSt!=*itSub) {
				//¸Ä±ä¶ÔÆëÎ»ÖÃºÍ³¤¶È
				std::advance(itSub, -vecSub[cnt-1]);
				cnt -= vecSub[cnt-1];
			}
			if(*itSt==*itSub)
				++ cnt, ++itSub;
			vecSub[locate] = (locate+1-cnt);
		}
		return *this;
	}
	//ËÑË÷º¯Êý£¬Ö§³ÖÎª¿ÕÆ¥Åä£¬ÈôÕÒµ½µü´úÆ÷ÎªÖ¸¶¨´®£¬Î´ÕÒµ½µü´úÆ÷¾ùÎª´®Î²ºó
	template<typename TyItT, typename TyItP, typename TyCtx= KmpContext<TyItT, TyItP>>
	typename std::enable_if<IsRemoveCVRefSame<KmpContext<TyItT, TyItP>, TyCtx>::value,
		std::pair<TyItT, TyItT>
	>::type Search(TyItT itTSt, TyItT itTEd, TyItP itPSt, TyItP itPEd,
		TyCtx &&ctx= TyCtx()) const
	{
		assert(std::distance(itPSt, itPEd)==vecSub.size());
		//³õÊ¼»¯
		if(ctx.cnt<0) {
			ctx.cnt = 0;
			ctx.it = itTSt;
			ctx.itSub = itPSt;
			ctx.itTFlag = itTSt;
		}
		//ÅÐ¶Ï¿Õ´®Æ¥Åä
		if(itPSt==itPEd) {
			auto ret = std::make_pair(ctx.itTFlag, ctx.it);
			++ ctx.it, ctx.itTFlag;
			return ret;
		}
		//Ö÷ÌåÑ­»·
		for(; ctx.it!=itTEd; ++ctx.it) {
			while(ctx.cnt!=0 && *ctx.it!=*ctx.itSub) {
				//¸Ä±ä¶ÔÆëÎ»ÖÃºÍ³¤¶È
				std::advance(ctx.itSub, -vecSub[ctx.cnt-1]);
				std::advance(ctx.itTFlag, vecSub[ctx.cnt-1]);
				ctx.cnt -= vecSub[ctx.cnt-1];
			}
			if(*ctx.it==*ctx.itSub) {
				++ ctx.cnt, ++ctx.itSub;
				if(ctx.cnt==vecSub.size()) {
					//ÏÂÒ»¸öÑ­»·Á¿
					++ ctx.it;
					auto ret = std::make_pair(ctx.itTFlag, ctx.it);
					//¸Ä±ä¶ÔÆëÎ»ÖÃºÍ³¤¶È
					std::advance(ctx.itSub, -vecSub[ctx.cnt-1]);
					std::advance(ctx.itTFlag, vecSub[ctx.cnt-1]);
					ctx.cnt -= vecSub[ctx.cnt-1];
					return ret;
				}
			}
			else
				++ ctx.itTFlag;
		}
		return std::make_pair(itTEd, itTEd);
	}
	//ÅÐ¶ÏÊÇ·ñÕÒµ½
	template<typename TyItT, typename TyItP>
	bool Judge(TyItT itTSt, TyItT itTEd, TyItP itPSt, TyItP itPEd) const
	{
		return Search(itTSt, itTEd, itPSt, itPEd).first!=itTEd;
	}
	//¼ÆÊýÕÒµ½´ÎÊý
	template<typename TyItT, typename TyItP>
	size_t Conut(TyItT itTSt, TyItT itTEd, TyItP itPSt, TyItP itPEd) const
	{
		KmpContext<TyItT, TyItP> ctx;
		size_t cnt = 0;
		for(; ; ++cnt) {
			if(Search(itTSt, itTEd, itPSt, itPEd, ctx).first==itTEd)
				break;
		}
		return cnt;
	}
};



//AC×Ô¶¯»úÄÚ²¿½ÚµãÀàÐÍ£¬Æ½ºâÊ÷ÊµÏÖ
template<typename TyMatch, typename TyData>
struct _AcMachineNode
{
	using RetDataType = TyData *;
	using ConstRetDataType = const TyData *;
	constexpr static bool bIsVoid = false;
public:
	TyData *pData = nullptr;
	std::map<TyMatch, _AcMachineNode> mapCh;//×ÓÊ÷
	size_t layer = 0;//µ±Ç°²ã
	_AcMachineNode *pBack = nullptr;//»ØËÝ½ÚµãÖ¸Õë
	_AcMachineNode *pFind = nullptr;//²éÕÒÓÐÐ§½ÚµãÖ¸Õë
public:
	_AcMachineNode()
		= default;
	~_AcMachineNode()
	{
		delete pData;
	}
	//¿½±´º¯Êý£¬¿½±´ºóÐèÒªÖØÐÂ¹¹ÔìÖ¸Õë
	_AcMachineNode(const _AcMachineNode &other)
	{
		operator =(other);
	}
	_AcMachineNode &operator =(const _AcMachineNode &other)
	{
		if(this!=&other) {
			mapCh = other.mapCh;
			//Èô¶Ô·½ÓÐÊý¾Ý
			if(other.pData) {
				if(pData)
					*pData = *other.pData;
				else
					Construct(other.data);
			}
			//Èô¶Ô·½Ã»Êý¾Ý
			else {
				if(pData) {
					delete pData;
					pData = nullptr;
				}
			}
			//¿½±´ÆäËûÊý¾Ý
			mapCh = other.mapCh;
			layer = other.layer;
			pBack = nullptr;
			pFind = nullptr;
		}
		return *this;
	}
	//¹¹ÔìÊý¾Ý
	template<typename ...Ty_S>
	void Construct(Ty_S &&...arg_s)
	{
		assert(pData==nullptr);
		pData = new TyData(std::forward<Ty_S>(arg_s)...);
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		delete pData;
		pData = nullptr;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾ÝÖ¸Õë
	static RetDataType GetData(_AcMachineNode *pt)
	{
		if(pt==nullptr)
			return nullptr;
		else
			return pt->pData;
	}
	static ConstRetDataType GetData(const _AcMachineNode *pt)
	{
		return GetData((_AcMachineNode *)pt);
	}
};
template<typename TyMatch>
struct _AcMachineNode<TyMatch, void>
{
	using RetDataType = bool;
	using ConstRetDataType = bool;
	constexpr static bool bIsVoid = true;
public:
	bool bData = false;//ÊÇ·ñÓÐÊý¾Ý
	std::map<TyMatch, _AcMachineNode> mapCh;//×ÓÊ÷
	size_t layer = 0;//µ±Ç°²ã
	_AcMachineNode *pBack = nullptr;//»ØËÝ½ÚµãÖ¸Õë
	_AcMachineNode *pFind = nullptr;//²éÕÒÓÐÐ§½ÚµãÖ¸Õë
public:
	_AcMachineNode()
		= default;
	_AcMachineNode(const _AcMachineNode &other)
	{
		operator =(other);
	}
	//¿½±´º¯Êý£¬¿½±´ºóÐèÒªÖØÐÂ¹¹ÔìÖ¸Õë
	_AcMachineNode &operator =(const _AcMachineNode &other)
	{
		if(this!=&other) {
			bData = other.bData;
			mapCh = other.mapCh;
			layer = other.layer;
			pBack = nullptr;
			pFind = nullptr;
		}
	}
	//¹¹ÔìÊý¾Ý
	void Construct()
	{
		bData = true;
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		bData = false;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾Ý±êÇ©
	static ConstRetDataType GetData(const _AcMachineNode *pt)
	{
		if(pt==nullptr)
			return false;
		else
			return pt->bData;
	}
};

//AC×Ô¶¯»úÄÚ²¿½ÚµãÀàÐÍ£¬¹þÏ£±íÊµÏÖ
template<typename TyMatch, typename TyData>
struct _UnordAcMachineNode
{
	using RetDataType = TyData *;
	using ConstRetDataType = const TyData *;
	constexpr static bool bIsVoid = false;
public:
	TyData *pData = nullptr;
	std::unordered_map<TyMatch, _UnordAcMachineNode> mapCh;//×ÓÊ÷
	size_t layer = 0;//µ±Ç°²ã
	_UnordAcMachineNode *pBack = nullptr;//»ØËÝ½ÚµãÖ¸Õë
	_UnordAcMachineNode *pFind = nullptr;//²éÕÒÓÐÐ§½ÚµãÖ¸Õë
public:
	_UnordAcMachineNode()
		= default;
	~_UnordAcMachineNode()
	{
		delete pData;
	}
	//¿½±´º¯Êý£¬¿½±´ºóÐèÒªÖØÐÂ¹¹ÔìÖ¸Õë
	_UnordAcMachineNode(const _UnordAcMachineNode &other)
	{
		operator =(other);
	}
	_UnordAcMachineNode &operator =(const _UnordAcMachineNode &other)
	{
		if(this!=&other) {
			mapCh = other.mapCh;
			//Èô¶Ô·½ÓÐÊý¾Ý
			if(other.pData) {
				if(pData)
					*pData = *other.pData;
				else
					Construct(other.data);
			}
			//Èô¶Ô·½Ã»Êý¾Ý
			else {
				if(pData) {
					delete pData;
					pData = nullptr;
				}
			}
			//¿½±´ÆäËûÊý¾Ý
			mapCh = other.mapCh;
			layer = other.layer;
			pBack = nullptr;
			pFind = nullptr;
		}
		return *this;
	}
	//¹¹ÔìÊý¾Ý
	template<typename ...Ty_S>
	void Construct(Ty_S &&...arg_s)
	{
		assert(pData==nullptr);
		pData = new TyData(std::forward<Ty_S>(arg_s)...);
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		delete pData;
		pData = nullptr;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾ÝÖ¸Õë
	static RetDataType GetData(_UnordAcMachineNode *pt)
	{
		if(pt==nullptr)
			return nullptr;
		else
			return pt->pData;
	}
	static ConstRetDataType GetData(const _UnordAcMachineNode *pt)
	{
		return GetData((_UnordAcMachineNode *)pt);
	}
};
template<typename TyMatch>
struct _UnordAcMachineNode<TyMatch, void>
{
	using RetDataType = bool;
	using ConstRetDataType = bool;
	constexpr static bool bIsVoid = true;
public:
	bool bData = false;//ÊÇ·ñÓÐÊý¾Ý
	std::unordered_map<TyMatch, _UnordAcMachineNode> mapCh;//×ÓÊ÷
	size_t layer = 0;//µ±Ç°²ã
	_UnordAcMachineNode *pBack = nullptr;//»ØËÝ½ÚµãÖ¸Õë
	_UnordAcMachineNode *pFind = nullptr;//²éÕÒÓÐÐ§½ÚµãÖ¸Õë
public:
	_UnordAcMachineNode()
		= default;
	_UnordAcMachineNode(const _UnordAcMachineNode &other)
	{
		operator =(other);
	}
	//¿½±´º¯Êý£¬¿½±´ºóÐèÒªÖØÐÂ¹¹ÔìÖ¸Õë
	_UnordAcMachineNode &operator =(const _UnordAcMachineNode &other)
	{
		if(this!=&other) {
			bData = other.bData;
			mapCh = other.mapCh;
			layer = other.layer;
			pBack = nullptr;
			pFind = nullptr;
		}
	}
	//¹¹ÔìÊý¾Ý
	void Construct()
	{
		bData = true;
	}
	//Çå³ýÊý¾Ý
	void Clear()
	{
		bData = false;
		mapCh.clear();
	}
	//»ñÈ¡Êý¾Ý±êÇ©
	static ConstRetDataType GetData(const _UnordAcMachineNode *pt)
	{
		if(pt==nullptr)
			return false;
		else
			return pt->bData;
	}
};

//binfile½Ó¿Ú
template<typename TyMatch, typename TyData>
inline BinWriteFile &operator <<(BinWriteFile &bwf, const _AcMachineNode<TyMatch, TyData> &node)
{
	_BinWriteTrieOrAcNodeAssist(bwf, node);
	return bwf;
}
template<typename TyMatch, typename TyData>
inline BinReadFile &operator >>(BinReadFile &brf, _AcMachineNode<TyMatch, TyData> &node)
{
	_BinReadTrieOrAcNodeAssist(brf, node);
	return brf;
}
template<typename TyMatch, typename TyData>
inline BinWriteFile &operator <<(BinWriteFile &bwf, const _UnordAcMachineNode<TyMatch, TyData> &node)
{
	_BinWriteTrieOrAcNodeAssist(bwf, node);
	return bwf;
}
template<typename TyMatch, typename TyData>
inline BinReadFile &operator >>(BinReadFile &brf, _UnordAcMachineNode<TyMatch, TyData> &node)
{
	_BinReadTrieOrAcNodeAssist(brf, node);
	return brf;
}

template<typename TyNode>
class _AcMachineTemplate;
//AC×Ô¶¯»úËÑË÷ÀàÉÏÏÂÎÄ
template<typename TyNode, typename TyIt>
struct _AcContextTemplate
{
	bool bFirst = true;//¼ÇÂ¼Ê×´ÎÆ¥Åä
	TyNode *pt = nullptr;//Ê÷½ÚµãÖ¸Õë
	TyNode *pFind = nullptr;//ÕýÔÚ²éÕÒÖ¸Õë
	TyIt it;//Æ¥Åä´®Ö¸Õë
};
template<typename TyNode, typename TyIt>
inline _AcContextTemplate<TyNode, TyIt> MakeAcContext(const _AcMachineTemplate<TyNode> &, TyIt)
{
	return _AcContextTemplate<TyNode, TyIt>();
}

//AC×Ô¶¯»úËÑË÷Àà£¬Ö§³ÖvoidÊý¾Ý
template<typename TyNode>
class _AcMachineTemplate
{
	template<typename TyNode2>
	friend BinWriteFile &operator <<(BinWriteFile &bwf, const _AcMachineTemplate<TyNode2> &acm);
	template<typename TyNode2>
	friend BinReadFile &operator >>(BinReadFile &brf, _AcMachineTemplate<TyNode2> &acm);

public:
	//¸¨ÖúÀàÐÍ
	using NodeType = TyNode;
	using RetDataType = typename NodeType::RetDataType;
	using ConstRetDataType = typename NodeType::ConstRetDataType;

private:
	NodeType *pRoot;//¸ù½Úµã
	bool bPrepare = true;

public:
	//Îå¿½±´
	_AcMachineTemplate():
		pRoot(new NodeType())
	{
	}
	~_AcMachineTemplate()
	{
		delete pRoot;
	}
	_AcMachineTemplate(const _AcMachineTemplate &other):
		_AcMachineTemplate()
	{
		operator =(other);
	}
	_AcMachineTemplate(_AcMachineTemplate &&other):
		_AcMachineTemplate()
	{
		operator =(std::move(other));
	}
	_AcMachineTemplate &operator =(const _AcMachineTemplate &other)
	{
		if(this!=&other) {
			*pRoot = *other.pRoot;
			bPrepare = false;
			if(other.bPrepare) {
				bPrepare = false;
				Prepare();
			}
		}
		return *this;
	}
	_AcMachineTemplate &operator =(_AcMachineTemplate &&other)
	{
		if(this!=&other) {
			std::swap(pRoot, other.pRoot);
			std::swap(bPrepare, other.bPrepare);
		}
		return *this;
	}
public:
	//×ö×¼±¸£¬ÔÚ½øÐÐÆ¥ÅäÖ®Ç°µ÷ÓÃ£¬Ò²¿ÉÊÖ¶¯µ÷ÓÃ
	void Prepare()
	{
		if(bPrepare)
			return;
		std::deque<NodeType *> deqNode;//½ÚµãÕ¹¿ª¶ÓÁÐ
												 //¹¹ÔìµÚÒ»²ã»ØËÝ
		deqNode.push_back(pRoot);
		//Õ¹¿ª¶ÓÁÐ
		while(!deqNode.empty()) {
			//Ã¿Ò»¸ö×Ó½Úµã
			for(auto &pr: deqNode.front()->mapCh) {
				//Ìí¼Ó¶ÓÁÐ
				deqNode.push_back(&pr.second);
				//Ñ°ÕÒ¸¸½Úµã
				auto pFindBack = deqNode.front();
				while(pFindBack->pBack!=nullptr) {
					pFindBack = pFindBack->pBack;
					auto res = pFindBack->mapCh.find(pr.first);
					if(res!=pFindBack->mapCh.end()) {
						pFindBack = &res->second;
						break;
					}
				}
				//¹¹Ôì¸¸Ö¸Õë
				pr.second.pBack = pFindBack;
				if(NodeType::GetData(pFindBack))
					pr.second.pFind = pFindBack;
				else
					pr.second.pFind = pFindBack->pFind;
				pr.second.layer = deqNode.front()->layer+1;
			}
			//µ¯³ö¶ÓÁÐ
			deqNode.pop_front();
		}
		bPrepare = true;
	}
	//Ìí¼ÓÆ¥Åä¹æÔò
	template<typename TyIt, typename ...Ty_S>
	bool EmplaceRule(TyIt itSt, TyIt itEd, Ty_S &&...arg_s)
	{
		bPrepare = false;
		NodeType *pNode = pRoot;
		while(itSt!=itEd) {
			auto res = pNode->mapCh.emplace(std::piecewise_construct,
				std::tie(*itSt), std::make_tuple());
			pNode = &res.first->second;
			bPrepare = bPrepare && !res.second;
			++ itSt;
		}
		if(NodeType::GetData(pNode))
			return false;
		pNode->Construct(std::forward<Ty_S>(arg_s)...);
		return true;
	}
	//Çå³ý½Úµã
	void Clear()
	{
		pRoot->Clear();
		bPrepare = true;
	}
	//µü´úËÑË÷º¯Êý£¬Êý¾ÝÀàÐÍ·µ»ØÖ¸Õë£¬¿ÕÀàÐÍ·µ»Ø²¼¶û
	//ÕÒ²»µ½·µ»Ø¿ÕÖ¸Õë£¬ÈôÕÒµ½·µ»ØÕÒµ½Î²ºóºÍ³¤¶È£¬Î´ÕÒµ½·µ»Ø´®Î²ºóºÍ0
	template<typename TyIt, typename TyCtx= _AcContextTemplate<TyNode, TyIt>>
	typename std::enable_if<IsRemoveCVRefSame<_AcContextTemplate<TyNode, TyIt>, TyCtx>::value,
		std::tuple<TyIt, size_t, RetDataType>
	>::type Search(TyIt itSt, TyIt itEd, TyCtx &&ctx= TyCtx())
	{
		//×¼±¸
		Prepare();
		//³õÊ¼»¯
		bool bFirst = false;
		if(ctx.bFirst) {
			ctx.bFirst = false;
			ctx.pt = pRoot;
			ctx.it = itSt;
			bFirst = true;
		}
		auto ret = std::make_tuple(ctx.it, (size_t)0, NodeType::GetData((NodeType *)nullptr));
		//ÅÐ¶ÏÊä³öÍ¬Ò»½Úµã½á¹û
		if(ctx.pFind!=nullptr) {
			std::get<1>(ret) = ctx.pFind->layer;
			std::get<2>(ret) = NodeType::GetData(ctx.pFind);
			ctx.pFind = ctx.pFind->pFind;
		}
		//ÈôÎªµÚÒ»´Î²éÕÒÇÒ´æÔÚ¿ÕÆ¥Åä
		else if(bFirst && NodeType::GetData(ctx.pt)) {
			std::get<1>(ret) = ctx.pt->layer;
			std::get<2>(ret) = NodeType::GetData(ctx.pt);
			ctx.pFind = ctx.pt->pFind;
		}
		//·ñÔò²éÕÒÏÂÒ»½Úµã
		else {
			//¶ÁÈë×Ö·ûÑ­»·
			while(ctx.it!=itEd) {
				//²éÕÒ»ØËÝ
				while(true) {
					auto res = ctx.pt->mapCh.find(*ctx.it);
					if(res!=ctx.pt->mapCh.end()) {
						ctx.pt = &res->second;
						break;
					}
					if(ctx.pt->pBack==nullptr)
						break;
					ctx.pt = ctx.pt->pBack;
				}
				++ ctx.it;
				//ÈôÕÒµ½´øÊý¾ÝµÄ½Úµã
				if(NodeType::GetData(ctx.pt)) {
					std::get<1>(ret) = ctx.pt->layer;
					std::get<2>(ret) = NodeType::GetData(ctx.pt);
					ctx.pFind = ctx.pt->pFind;
					break;
				}
				//ÈôÕÒµ½´ø²éÕÒÊý¾ÝµÄ½Úµã
				else if(ctx.pt->pFind!=nullptr) {
					ctx.pFind = ctx.pt->pFind;
					std::get<1>(ret) = ctx.pFind->layer;
					std::get<2>(ret) = NodeType::GetData(ctx.pFind);
					ctx.pFind = ctx.pFind->pFind;
					break;
				}
			}
			std::get<0>(ret) = ctx.it;
		}
		return ret;
	}
	//ÕÒµ½µÚÒ»¸ö£¬Æ¥ÅäÎ²ºó¿¼Ç°ÓÅÏÈ£¬Æ¥ÅäÊ×²¿¿¿Ç°ÓÅÏÈ
	template<typename TyIt>
	RetDataType Judge(TyIt itSt, TyIt itEd)
	{
		return std::get<2>(Search(itSt, itEd));
	}
	//¼ÆÊýÕÒµ½´ÎÊý
	template<typename TyIt>
	RetDataType Count(TyIt itSt, TyIt itEd)
	{
		_AcContextTemplate<TyNode, TyIt> ctx;
		size_t cnt = 0;
		for(; ; ++cnt) {
			if(std::get<2>(Search(itSt, itEd, ctx)))
				break;
		}
		return cnt;
	}
};

//binfile½Ó¿Ú
template<typename TyNode>
inline BinWriteFile &operator <<(BinWriteFile &bwf, const _AcMachineTemplate<TyNode> &acm)
{
	return bwf <<(uint8_t)acm.bPrepare
		<<*acm.pRoot;
}
template<typename TyNode>
inline BinReadFile &operator >>(BinReadFile &brf, _AcMachineTemplate<TyNode> &acm)
{
	uint8_t bPrepare;
	brf >>bPrepare;
	acm.bPrepare = false;
	brf >>*acm.pRoot;
	if(bPrepare)
		acm.Prepare();
	return brf;
}

//¶¨ÒåËÑË÷Ê÷µÄÆ½ºâÊ÷ÊµÏÖ
template<typename TyMatch, typename TyData= void>
using AcMachine = _AcMachineTemplate<_AcMachineNode<TyMatch, TyData>>;
template<typename TyMatch, typename TyData, typename TyIt>
using AcContext = _AcContextTemplate<_AcMachineNode<TyMatch, TyData>, TyIt>;
//¶¨ÒåËÑË÷Ê÷µÄ¹þÏ£±íÊµÏÖ
template<typename TyMatch, typename TyData= void>
using UnordAcMachine = _AcMachineTemplate<_UnordAcMachineNode<TyMatch, TyData>>;
template<typename TyMatch, typename TyData, typename TyIt>
using UnordAcContext = _AcContextTemplate<_UnordAcMachineNode<TyMatch, TyData>, TyIt>;




//Ö»ÄÜÌí¼ÓÔªËØµÄÍ¼£¬Ö§³ÖvoidÊý¾ÝÀàÐÍ
//ÄÚ²¿Îª¶þÎ¬Êý×éÊµÏÖ
//½Úµãµü´úÆ÷ÔÊÐíÊ¹ÓÃÕûÐÎË÷Òý¹¹Ôì
//ClearºÍAssign»áµ¼ÖÂµü´úÆ÷Ê§Ð§
//ÈÝÆ÷¸´ÖÆ¡¢ÒÆ¶¯¡¢½»»»ºóµü´úÆ÷¾ùÖ¸Ïò¾ÉÈÝÆ÷£¬ÔÊÐíµü´úÆ÷¸üÐÂÈÝÆ÷Ö¸Õë
//TODO: Ïë½á¹¹Éè¼Æ£¬³¢ÊÔ½«½ÚµãÓë±ß¶ÔÓ¦µ½ÕûÐÎË÷ÒýÉÏ
template<typename TyNodeData, typename TyEdgeData>
class AddonlyGraph
{
private:
	//±ß½á¹¹Ìå
	struct EdgeStruct:
		public ValueHolder<TyEdgeData>
	{
		size_t index;//±ß±àºÅ
						 //½ÓÊÕÊý¾ÝÀàÐÍÄ£°å²ÎÊýµÄ¹¹Ôì
		template<typename ...Tys>
		EdgeStruct(size_t o_index, Tys &&...args):
			ValueHolder<TyEdgeData>(std::forward<Tys>(args)...), index(o_index)
		{
		}
	};
	//½Úµã½á¹¹Ìå
	struct NodeStruct:
		public ValueHolder<TyNodeData>
	{
		std::vector<EdgeStruct> vecEdge;//³ö±ßÊý×é
												  //½ÓÊÕ×óÖµ»òÓÒÖµµÄÊý¾Ý¹¹Ôì
		template<typename ...Tys>
		NodeStruct(Tys &&...args):
			ValueHolder<TyNodeData>(std::forward<Tys>(args)...)
		{
		}
	};

private://¸¨ÖúÀàÐÍ¶¨Òå
	using GraphType = AddonlyGraph<TyNodeData, TyEdgeData>;//Í¼ÀàÐÍ
public:
	using NodeDataType = TyNodeData;//½ÚµãÊý¾ÝÀàÐÍ
	using EdgeDataType = TyEdgeData;//±ßÊý¾ÝÀàÐÍ

private://µü´úÆ÷Ä£°å¶¨Òå
		  //½Úµãµü´úÆ÷Ä£°å¸¨ÖúÀà
	template<typename GraphTypeMy, typename TyNodeDataMy, typename TyNodeDataPure>
	class TemplateNodeIterAssist;
	//½Úµãµü´úÆ÷Ä£°å
	template<typename GraphTypeMy, typename TyNodeDataMy, typename TyEdgeDataMy>
	class TemplateNodeIter;
	//±ßµü´úÆ÷Ä£°å¸¨ÖúÀà
	template<typename GraphTypeMy, typename TyNodeDataMy, typename TyNodeDataPure>
	class TemplateEdgeIterAssist;
	//±ßµü´úÆ÷Ä£°å
	template<typename GraphTypeMy, typename TyNodeDataMy, typename TyEdgeDataMy>
	class TemplateEdgeIter;

private://µü´úÆ÷Ä£°åÖ¸¶¨ÀàÐÍ¶¨Òå
		  //½Úµãµü´úÆ÷¸¨ÖúÀàÖ¸¶¨
	using NodeIterAssist = TemplateNodeIter<GraphType, TyNodeData, TyNodeData>;
	//½Úµã³£µü´úÆ÷¸¨ÖúÀàÖ¸¶¨
	using NodeConstIterAssist = TemplateNodeIter<
		const GraphType, const TyNodeData, TyNodeData>;
	//±ßµü´úÆ÷¸¨ÖúÀàÖ¸¶¨
	using EdgeIterAssist = TemplateEdgeIter<GraphType, TyEdgeData, TyEdgeData>;
	//±ß³£µü´úÆ÷¸¨ÖúÀàÖ¸¶¨
	using EdgeConstIterAssist = TemplateEdgeIter<
		const GraphType, const TyEdgeData, TyEdgeData>;
public:
	//½Úµãµü´úÆ÷ÀàÖ¸¶¨
	using NodeIter = TemplateNodeIter<GraphType, TyNodeData, TyEdgeData>;
	//½Úµã³£µü´úÆ÷ÀàÖ¸¶¨
	using NodeConstIter = TemplateNodeIter<
		const GraphType, const TyNodeData, const TyEdgeData>;
	//±ßµü´úÆ÷ÀàÖ¸¶¨
	using EdgeIter = TemplateEdgeIter<GraphType, TyNodeData, TyEdgeData>;
	//±ß³£µü´úÆ÷ÀàÖ¸¶¨
	using EdgeConstIter = TemplateEdgeIter<
		const GraphType, const TyNodeData, const TyEdgeData>;

public://ÓÑÔªÉùÃ÷
	friend NodeIter;
	friend NodeIterAssist;
	friend NodeConstIter;
	friend NodeConstIterAssist;
	friend EdgeIter;
	friend EdgeIterAssist;
	friend EdgeConstIter;
	friend EdgeConstIterAssist;

private://³ÉÔ±Êý¾Ý
	std::vector<NodeStruct> vecNode;//½ÚµãÊý×é

public://»ù±¾º¯Êý
		 //Ä¬ÈÏ¹¹Ôì
	AddonlyGraph()
		= default;
	//¶à¸öÊý¾Ý¹¹Ôì
	template<typename ...Ty_S>
	explicit AddonlyGraph(size_t size, const Ty_S &...arg_s)
	{
		Assign(size, arg_s...);
	}
public://³ÉÔ±º¯Êý
		 //¸³Öµº¯Êý
	template<typename ...Ty_S>
	AddonlyGraph &Assign(size_t size, const Ty_S &...arg_s)
	{
		vecNode.assign(size, NodeStruct(arg_s...));
		return *this;
	}
	//Ìí¼Ó½Úµã
	template<typename TyNodeDataMy>
	typename std::enable_if<std::is_convertible<TyNodeDataMy &&, TyNodeData>::value, NodeIter
	>::type AddNode(TyNodeDataMy &&data)
	{
		return EmplaceNode(std::forward<TyNodeDataMy>(data));
	}
	template<typename ...Tys>
	NodeIter EmplaceNode(Tys &&...args)
	{
		vecNode.emplace_back(std::forward<Tys>(args)...);
		return NodeIter(this, vecNode.size()-1);
	}
	//Ìí¼Ó±ß
	template<typename TyEdgeDataMy>
	typename std::enable_if<std::is_convertible<TyEdgeDataMy &&, TyEdgeData>::value, EdgeIter
	>::type AddEdge(NodeIter fromNode, NodeIter toNode, TyEdgeDataMy &&data)
	{
		return EmplaceEdge(fromNode, toNode, std::forward<TyEdgeDataMy>(data));
	}
	//¹¹Ôì±ß
	template<typename ...Tys>
	EdgeIter EmplaceEdge(NodeIter fromNode, NodeIter toNode, Tys &&...args)
	{
		vecNode[fromNode.idxNode].vecEdge.emplace_back(
			toNode.idxNode, std::forward<Tys>(args)...);
		return EdgeIter(this, fromNode.idxNode, vecNode[fromNode.idxNode].vecEdge.size()-1);
	}
	//Ìí¼ÓË«Ïò±ß
	template<typename TyEdgeDataMy>
	typename std::enable_if<std::is_convertible<TyEdgeDataMy &&, TyEdgeData>::value,
		std::pair<EdgeIter, EdgeIter>
	>::type AddDoublyEdge(NodeIter fromNode, NodeIter toNode,
		TyEdgeDataMy &&dataGo, TyEdgeDataMy &&dataRev)
	{
		auto it = AddAdge(fromNode, toNode, std::forward<TyEdgeDataMy>(dataGo));
		return std::make_pair(it,
			AddAdge(toNode, fromNode, std::forward<TyEdgeDataMy>(dataRev)));
	}
private:
	//¹¹ÔìË«Ïò±ß¸¨Öúº¯Êý
	template<typename TyTupGo, typename TyTupRev, size_t ...c_idxGo_s, size_t ...c_idxRev_s>
	std::pair<EdgeIter, EdgeIter> EmplaceDoublyEdgeAssist(NodeIter fromNode,
		NodeIter toNode, TyTupGo &&tupGo, TyTupRev &&tupRev,
		IndexSequence<c_idxGo_s...>, IndexSequence<c_idxRev_s...>)
	{
		static auto funcGo = std::mem_fn(&AddonlyGraph::EmplaceEdge<
			typename TypeAttachAttribute<TyTupGo &&, typename std::tuple_element<
			c_idxGo_s, typename RemoveCVRef<TyTupGo>::type>::type>::type...>);
		static auto funcRev = std::mem_fn(&AddonlyGraph::EmplaceEdge<
			typename TypeAttachAttribute<TyTupRev &&, typename std::tuple_element<
			c_idxRev_s, typename RemoveCVRef<TyTupRev>::type>::type>::type...>);
		auto itGo = TupleInvoke(funcGo,
			std::tuple_cat(std::forward_as_tuple(this, fromNode, toNode), std::forward<TyTupGo>(tupGo)));
		auto itRev = TupleInvoke(funcRev,
			std::tuple_cat(std::forward_as_tuple(this, toNode, fromNode), std::forward<TyTupRev>(tupRev)));
		return std::make_pair(itGo, itRev);
	}
public:
	//¹¹ÔìË«Ïò±ß£¬ÔÚtupleÀïÃæ·Å¹¹Ôì²ÎÊý
	template<typename TyTupGo, typename TyTupRev>
	std::pair<EdgeIter, EdgeIter> EmplaceDoublyEdge(NodeIter fromNode,
		NodeIter toNode, TyTupGo &&tupGo, TyTupRev &&tupRev)
	{
		return EmplaceDoublyEdgeAssist(fromNode, toNode,
			std::forward<TyTupGo>(tupGo), std::forward<TyTupRev>(tupRev),
			MakeIndexSequence<std::tuple_size<
			typename std::remove_reference<TyTupGo>::type>::value>(),
			MakeIndexSequence<std::tuple_size<
			typename std::remove_reference<TyTupRev>::type>::value>());
	}
	//Çå³ýÍ¼
	void Clear()
	{
		vecNode.clear();
	}
	//µü´ú½Úµã
	NodeIter NodeBegin()
	{
		return NodeIter(this, 0);
	}
	NodeConstIter NodeBegin() const
	{
		return NodeConstIter(this, 0);
	}
	NodeIter begin()
	{
		return NodeBegin();
	}
	NodeConstIter begin() const
	{
		return NodeBegin();
	}
	NodeConstIter NodeConstBegin()
	{
		return NodeBegin();
	}
	NodeIter NodeEnd()
	{
		return NodeIter(this, vecNode.size());
	}
	NodeConstIter NodeEnd() const
	{
		return NodeConstIter(this, vecNode.size());
	}
	NodeIter end()
	{
		return NodeEnd();
	}
	NodeConstIter end() const
	{
		return NodeEnd();
	}
	NodeConstIter NodeConstEnd()
	{
		return NodeEnd();
	}
	//½ÚµãÊý
	size_t NodeSize() const
	{
		return vecNode.size();
	}
};

//µãµü´úÆ÷Ä£°å¸¨ÖúÀà
template<typename TyNodeData, typename TyEdgeData>
template<typename GraphTypeMy, typename TyNodeDataMy, typename TyNodeDataPure>
class AddonlyGraph<TyNodeData, TyEdgeData>::TemplateNodeIterAssist:
	public std::iterator<std::bidirectional_iterator_tag, TyNodeDataMy,
	ptrdiff_t, TyNodeDataMy *, TyNodeDataMy &>
{
protected://³ÉÔ±Êý¾Ý
	GraphTypeMy *pGraph;//Í¼Ö¸Õë
	size_t idxNode;//½Úµã

public://»ù±¾º¯Êý
		 //Ä¬ÈÏ¹¹Ôì
	TemplateNodeIterAssist():
		pGraph(nullptr), idxNode(0)
	{
	}
	//¹¹Ôìº¯Êý£¬ÔÊÐíÍâ²¿½øÐÐ¹¹Ôì
	explicit TemplateNodeIterAssist(GraphTypeMy *o_graph, size_t o_idxNode):
		pGraph(o_graph), idxNode(o_idxNode)
	{
	}
public://ÔËËã·ûÖØÔØ
		 //½âÒýÓÃ²Ù×÷
	TyNodeDataMy &operator *() const
	{
		return pGraph->vecNode[idxNode].hold;
	}
	TyNodeDataMy *operator ->() const
	{
		return &operator *();
	}
};
//µãµü´úÆ÷Ä£°å¸¨ÖúÀàÌØ»¯
template<typename TyNodeData, typename TyEdgeData>
template<typename GraphTypeMy, typename TyNodeDataMy>
class AddonlyGraph<TyNodeData, TyEdgeData>::TemplateNodeIterAssist<
	GraphTypeMy, TyNodeDataMy, void
>: public std::iterator<std::bidirectional_iterator_tag, TyNodeDataMy,
	ptrdiff_t, void, void>
{
protected://³ÉÔ±Êý¾Ý
	GraphTypeMy *pGraph;//Í¼Ö¸Õë
	size_t idxNode;//½Úµã

public://»ù±¾º¯Êý
		 //Ä¬ÈÏ¹¹Ôì
	TemplateNodeIterAssist():
		pGraph(nullptr), idxNode(0)
	{
	}
	//¹¹Ôìº¯Êý£¬ÔÊÐíÍâ²¿½øÐÐ¹¹Ôì
	explicit TemplateNodeIterAssist(GraphTypeMy *o_graph, size_t o_idxNode):
		pGraph(o_graph), idxNode(o_idxNode)
	{
	}
};

//½Úµãµü´úÆ÷Ä£°åÀà
template<typename TyNodeData, typename TyEdgeData>
template<typename GraphTypeMy, typename TyNodeDataMy, typename TyEdgeDataMy>
class AddonlyGraph<TyNodeData, TyEdgeData>::TemplateNodeIter:
	public TemplateNodeIterAssist<GraphTypeMy, TyNodeDataMy, TyNodeData>
{
public://ÓÑÔª
	friend AddonlyGraph<TyNodeData, TyEdgeData>;
	friend NodeIter;
	friend NodeConstIter;
	friend EdgeIter;
	friend EdgeConstIter;
private://¸¨ÖúÀàÐÍ
		  //¶ÔÓ¦µÄ±ßµü´úÆ÷
	using EdgeIterMy = TemplateEdgeIter<GraphTypeMy, TyNodeDataMy, TyEdgeDataMy>;

public://»ù±¾º¯Êý
		 //Ä¬ÈÏ¹¹Ôì
	TemplateNodeIter()
		= default;
	//¼Ì³Ð¸¨ÖúÀàµÄ¹¹Ôìº¯Êý
	using TemplateNodeIterAssist<GraphTypeMy, TyNodeDataMy, TyNodeData>::TemplateNodeIterAssist;
	//ÓÉ·Ç³£Àà¹¹Ôì³£Àà
	TemplateNodeIter(const NodeIter &other)
	{
		operator =(other);
	}
	TemplateNodeIter &operator =(const NodeIter &other)
	{
		if((void *)this!=(void *)&other) {
			this->pGraph = other.pGraph;
			this->idxNode = other.idxNode;
		}
		return *this;
	}
public://ÔËËã·ûÖØÔØ
		 //µÝÔöµÝ¼õ²Ù×÷
	TemplateNodeIter &operator ++()
	{
		++ this->idxNode;
		return *this;
	}
	TemplateNodeIter operator ++(int)
	{
		TemplateNodeIter ret(*this);
		operator ++();
		return ret;
	}
	TemplateNodeIter &operator --()
	{
		-- this->idxNode;
		return *this;
	}
	TemplateNodeIter operator --(int)
	{
		TemplateNodeIter ret(*this);
		operator --();
		return ret;
	}
	//±È½Ï²Ù×÷
	template<typename GraphTypeOther, typename TyNodeDataOther, typename TyEdgeDataOther>
	bool operator ==(const TemplateNodeIter<
		GraphTypeOther, TyNodeDataOther, TyEdgeDataOther> &other) const
	{
		return this->idxNode==other.idxNode;
	}
	template<typename GraphTypeOther, typename TyNodeDataOther, typename TyEdgeDataOther>
	bool operator !=(const TemplateNodeIter<
		GraphTypeOther, TyNodeDataOther, TyEdgeDataOther> &other) const
	{
		return !operator ==(other);
	}
public://³ÉÔ±º¯Êý
		 //³ö±ßµü´ú²Ù×÷
	EdgeIterMy EdgeBegin() const
	{
		return EdgeIterMy(this->pGraph, this->idxNode, 0);
	}
	EdgeIterMy begin() const
	{
		return EdgeBegin();
	}
	EdgeConstIter EdgeConstBegin() const
	{
		return EdgeBegin();
	}
	EdgeIterMy EdgeEnd() const
	{
		return EdgeIterMy(this->pGraph, this->idxNode, this->pGraph->vecNode[this->idxNode].vecEdge.size());
	}
	EdgeIterMy end() const
	{
		return EdgeEnd();
	}
	EdgeConstIter EdgeConstEnd() const
	{
		return EdgeEnd();
	}
	//´¦±ßÊý
	size_t EdgeSize() const
	{
		return this->pGraph->vecNode[this->idxNode].size();
	}
	//¸Ä±äÖ¸Ïò²Ù×÷
	void ChangePointer(GraphTypeMy *o_graph)
	{
		this->pGraph = o_graph;
	}
};

//±ß´úÆ÷Ä£°å¸¨ÖúÀà
template<typename TyNodeData, typename TyEdgeData>
template<typename GraphTypeMy, typename TyEdgeDataMy, typename TyEdgeDataPure>
class AddonlyGraph<TyNodeData, TyEdgeData>::TemplateEdgeIterAssist:
	public std::iterator<std::bidirectional_iterator_tag, TyEdgeDataMy,
	ptrdiff_t, TyEdgeDataMy *, TyEdgeDataMy &>
{
protected://³ÉÔ±Êý¾Ý
	GraphTypeMy *pGraph;//Í¼Ö¸Õë
	size_t idxNode;//½ÚµãÐòºÅ
	size_t idxEdge;//±ßÐòºÅ

public://»ù±¾º¯Êý
		 //Ä¬ÈÏ¹¹Ôì
	TemplateEdgeIterAssist():
		pGraph(nullptr), idxNode(0), idxEdge(0)
	{
	}
protected:
	//¹¹Ôìº¯Êý£¬²»ÔÊÐíÍâ²¿½øÐÐ¹¹Ôì
	explicit TemplateEdgeIterAssist(GraphTypeMy *o_graph, size_t o_idxNode, size_t o_idxEdge):
		pGraph(o_graph), idxNode(o_idxNode), idxEdge(o_idxEdge)
	{
	}
public://ÔËËã·ûÖØÔØ
		 //½âÒýÓÃ²Ù×÷
	TyEdgeDataMy &operator *() const
	{
		return pGraph->vecNode[idxNode].vecEdge[idxEdge].hold;
	}
	TyEdgeDataMy *operator ->() const
	{
		return &operator *();
	}
};
//±ßµü´úÆ÷Ä£°å¸¨ÖúÀàÌØ»¯
template<typename TyNodeData, typename TyEdgeData>
template<typename GraphTypeMy, typename TyEdgeDataMy>
class AddonlyGraph<TyNodeData, TyEdgeData>::TemplateEdgeIterAssist<
	GraphTypeMy, TyEdgeDataMy, void
>: public std::iterator<std::bidirectional_iterator_tag, TyEdgeDataMy,
	ptrdiff_t, void, void>
{
protected://³ÉÔ±Êý¾Ý
	GraphTypeMy *pGraph;//Í¼Ö¸Õë
	size_t idxNode;//½ÚµãÐòºÅ
	size_t idxEdge;//±ßÐòºÅ

public://»ù±¾º¯Êý
		 //Ä¬ÈÏ¹¹Ôì
	TemplateEdgeIterAssist():
		pGraph(nullptr), idxNode(0), idxEdge(0)
	{
	}
protected:
	//¹¹Ôìº¯Êý£¬²»ÔÊÐíÍâ²¿½øÐÐ¹¹Ôì
	explicit TemplateEdgeIterAssist(GraphTypeMy *o_graph, size_t o_idxNode, size_t o_idxEdge):
		pGraph(o_graph), idxNode(o_idxNode), idxEdge(o_idxEdge)
	{
	}
};

//±ßµü´úÆ÷Ä£°åÀà
template<typename TyNodeData, typename TyEdgeData>
template<typename GraphTypeMy, typename TyNodeDataMy, typename TyEdgeDataMy>
class AddonlyGraph<TyNodeData, TyEdgeData>::TemplateEdgeIter:
	public TemplateEdgeIterAssist<GraphTypeMy, TyEdgeDataMy, TyEdgeData>
{
public://ÓÑÔª
	friend AddonlyGraph<TyNodeData, TyEdgeData>;
	friend NodeIter;
	friend NodeConstIter;
	friend EdgeIter;
	friend EdgeConstIter;
private://¸¨ÖúÀàÐÍ
		  //¶ÔÓ¦µÄ½Úµãµü´úÆ÷
	using NodeIterMy = TemplateNodeIter<GraphTypeMy, TyNodeDataMy, TyEdgeDataMy>;

public://»ù±¾º¯Êý
		 //¼Ì³Ð¸¨ÖúÀàµÄ¹¹Ôìº¯Êý
	using TemplateEdgeIterAssist<GraphTypeMy, TyEdgeDataMy, TyEdgeData>::TemplateEdgeIterAssist;
	//ÓÉ·Ç³£Àà¹¹Ôì³£Àà
	TemplateEdgeIter(const EdgeIter &other)
	{
		operator =(other);
	}
	TemplateEdgeIter &operator =(const EdgeIter &other)
	{
		if((void *)this!=(void *)&other) {
			this->pGraph = other.pGraph;
			this->idxNode = other.idxNode;
			this->idxEdge = other.idxEdge;
		}
		return *this;
	}
public://ÔËËã·ûÖØÔØ
		 //µÝÔöµÝ¼õÔì×÷
	TemplateEdgeIter &operator ++()
	{
		++ this->idxEdge;
		return *this;
	}
	TemplateEdgeIter operator ++(int)
	{
		TemplateEdgeIter ret(*this);
		operator ++();
		return ret;
	}
	TemplateEdgeIter &operator --()
	{
		-- this->idxEdge;
		return *this;
	}
	TemplateEdgeIter operator --(int)
	{
		TemplateEdgeIter ret(*this);
		operator --();
		return ret;
	}
	//±È½Ï²Ù×÷
	template<typename GraphTypeOther, typename TyNodeDataOther, typename TyEdgeDataOther>
	bool operator ==(const TemplateEdgeIter<
		GraphTypeOther, TyNodeDataOther, TyEdgeDataOther> &other) const
	{
		return this->idxNode==other.idxNode && this->idxEdge==other.idxEdge;
	}
	template<typename GraphTypeOther, typename TyNodeDataOther, typename TyEdgeDataOther>
	bool operator !=(const TemplateEdgeIter<
		GraphTypeOther, TyNodeDataOther, TyEdgeDataOther> &other) const
	{
		return !operator ==(other);
	}
public://³ÉÔ±º¯Êý
		 //»ñÈ¡Á¬½Ó½Úµã
	NodeIterMy NodeFrom() const
	{
		return NodeIterMy(this->pGraph, this->idxNode);
	}
	NodeConstIter NodeConstFrom() const
	{
		return NodeFrom();
	}
	NodeIterMy NodeTo() const
	{
		return NodeIterMy(this->pGraph, this->pGraph->vecNode[this->idxNode].vecEdge[this->idxEdge].index);
	}
	NodeIterMy NodeConstTo() const
	{
		return NodeTo();
	}
};


/*Í¼·ÖÀàÉèÏë

1£¬Ö»ÄÜÌí¼ÓµÄÍ¼£º
AddNode
AddEdge
Clear
NodeBegin
NodeEnd
NodeIter::EdgeBegin
NodeIter::EdgeEnd
NodeIter::operator ++
NodeIter::operator --
NodeIter::operator *
EdgeIter::NodeFrom
EdgeIter::NodeTo
EdgeIter::operator ++
NodeIter::operator --
EdgeIter::operator *

2£¬Ö»ÄÜÌí¼ÓµÄÍ¼£¬¿ÉÒÔ²é¿´À´Ô´±ß£º
È«²¿1º¯Êý
NodeIter::EdgeFromBegin
NodeIter::EdgeFromEnd

3£¬Ö»ÄÜÌí¼ÓµÄÍ¼£¬¿ÉÒÔ¸ù¾ÝÁ½¸ö¶¥µãÕÒÀ´Ô´
È«²¿1º¯Êý
GetEdge

4£¬Ö»ÄÜÌí¼ÓµÄÍ¼£¬´øÓÐÁ½ÕßÊôÐÔ
È«²¿2£¬3º¯Êý

5£¬¿ÉÒÔÐÞ¸ÄµÄÍ¼
È«²¿1º¯Êý
È«²¿2º¯Êý
DelNode
DelEdge

6£¬ÔÚ5»ù´¡ÉÏÔö¼Ó
3¹¦ÄÜ£¬»¹Ã»ÏëºÃ

*/




//µ¥´¿ÐÎ½á¹û×´Ì¬
enum class SimplexStatus
{
	result, infeasible, unbounded,//ÓÐ½á¹û£¬ÎÞ½â£¬ÎÞ½ç
};

//µ¥´¿ÐÎ¸¨ÖúÐý×ª
template<typename Ty>
inline void _SimplexAssistPivot(std::vector<std::vector<Ty>> &aMatrix, std::vector<Ty> &bVector,
	std::vector<Ty> &cVector, Ty &vValue, std::vector<int> &bSet, std::vector<int> &nSet,
	int lIndex, int eIndex)
{
	int mNum= (int)bVector.size(), nNum= (int)cVector.size();//Ô¼ÊøÊý£¬±äÁ¿Êý
																				//×ª»»»»³ö±äÁ¿ËùÔÚÐÐÒ»ÐÐ
	aMatrix[lIndex][eIndex] = 1/aMatrix[lIndex][eIndex];
	bVector[lIndex] *= aMatrix[lIndex][eIndex];
	for(int j=0; j<nNum; ++j) {
		if(j==eIndex)
			continue;
		aMatrix[lIndex][j] *= aMatrix[lIndex][eIndex];
	}
	//×ª»»¾ØÕóÆäËûÐÐ
	for(int i=0; i<mNum; ++i) {
		if(i==lIndex)
			continue;
		bVector[i] -= aMatrix[i][eIndex]*bVector[lIndex];
		for(int j=0; j<nNum; ++j) {
			if(j==eIndex)
				continue;
			aMatrix[i][j] -= aMatrix[i][eIndex]*aMatrix[lIndex][j];
		}
		aMatrix[i][eIndex] *= -aMatrix[lIndex][eIndex];
	}
	//×ª»»Ä¿±êº¯Êý
	vValue += cVector[eIndex]*bVector[lIndex];
	for(int j=0; j<nNum; ++j) {
		if(j==eIndex)
			continue;
		cVector[j] -= cVector[eIndex]*aMatrix[lIndex][j];
	}
	cVector[eIndex] *= -aMatrix[lIndex][eIndex];
	//×ª»»Ë÷Òý
	std::swap(bSet[lIndex], nSet[eIndex]);
}

//µ¥´¿ÐÎ¸¨ÖúËã·¨Ñ­»·Ö÷Ìå
template<typename Ty>
inline SimplexStatus _SimplexAssistLoop(std::vector<std::vector<Ty>> &aMatrix, std::vector<Ty> &bVector,
	std::vector<Ty> &cVector, Ty &vValue, std::vector<int> &bSet, std::vector<int> &nSet, int ulp)
{
	int mNum= (int)bVector.size(), nNum= (int)cVector.size();//Ô¼ÊøÊý£¬±äÁ¿Êý
																				//µ¥´¿ÐÎÑ­»·
	while(true) {
		//ÕÒ»»Èë»»³ö±äÁ¿Ñ­»·
		bool bFirst = true;
		int lIndex, eIndex;
		while(true) {
			//ÕÒ»»Èë±äÁ¿
			eIndex = -1;
			Ty maxC = -std::numeric_limits<Ty>::infinity();
			for(int j=0; j<nNum; ++j) {
				if(bFirst) {
					if(cVector[j]>maxC) {
						eIndex = j;
						maxC = cVector[j];
					}
				}
				else {
					//ÍË»¯Ê±Ñ¡±êºÅ×îÐ¡
					if(FloatGT(cVector[j], 0.0, true, ulp) && (eIndex==-1 || nSet[j]<nSet[eIndex])) {
						eIndex = j;
						maxC = cVector[j];
					}
				}
			}
			if(FloatLTE(maxC, 0.0, true, ulp))
				return SimplexStatus::result;
			//ÕÒ»»³ö±äÁ¿
			lIndex = -1;
			Ty minB = std::numeric_limits<Ty>::infinity();
			for(int i=0; i<mNum; ++i) {
				if(FloatGT(aMatrix[i][eIndex], 0.0, true, ulp)) {
					Ty tmp = bVector[i]/aMatrix[i][eIndex];
					if(tmp<minB) {
						minB = tmp;
						lIndex = i;
					}
				}
			}
			//ÅÐ¶ÏÎÞ½ç
			if(lIndex==-1) {
				vValue = std::numeric_limits<Ty>::infinity();
				return SimplexStatus::unbounded;
			}
			//ÍË»¯Ôò¼ÌÐøÑ­»·
			if(bFirst && FloatEQ(minB, 0.0, true, ulp))
				bFirst = false;
			else
				break;
		}
		//Ðý×ª
		_SimplexAssistPivot(aMatrix, bVector, cVector, vValue, bSet, nSet, lIndex, eIndex);
	}
}

//µ¥´¿ÐÎ¸¨Öú³õÊ¼»¯
template<typename Ty>
inline SimplexStatus _SimplexAssistInit(std::vector<std::vector<Ty>> &aMatrix, std::vector<Ty> &bVector,
	std::vector<Ty> &cVector, Ty &vValue, std::vector<int> &bSet, std::vector<int> &nSet, int ulp)
{
	int mNum= (int)bVector.size(), nNum= (int)cVector.size();//Ô¼ÊøÊý£¬±äÁ¿Êý
																				//ÅÐ¶Ï»ù±¾½â¿ÉÐÐÐÔ
	bool bPoss = true;
	bSet.reserve(mNum);
	nSet.reserve(nNum+1);
	for(int j=0; j<nNum; ++j)
		nSet.push_back(j);
	Ty minB = std::numeric_limits<Ty>::infinity();
	int lIndex = -1;
	for(int i=0; i<mNum; ++i) {
		bSet.push_back(nNum+i);
		if(bVector[i]<minB) {
			minB = bVector[i];
			lIndex = i;
		}
	}
	if(FloatGTE(minB, 0.0, true, ulp))
		return SimplexStatus::result;
	//¹¹Ôì¸¨Öú¹æ»®
	Ty vValue2 = 0;
	std::vector<Ty> cVector2(nNum+1, 0);
	cVector2[nNum] = -1;
	for(int i=0; i<mNum; ++i)
		aMatrix[i].push_back(-1);
	nSet.push_back(-1);
	//³õÊ¼Ðý×ª²¢Çó½â
	_SimplexAssistPivot(aMatrix, bVector, cVector2, vValue2, bSet, nSet, lIndex, nNum);
	SimplexStatus status = _SimplexAssistLoop(aMatrix, bVector, cVector2, vValue2, bSet, nSet, ulp);
	assert(status==SimplexStatus::result);
	//ÅÐ¶Ï¿ÉÐÐÐÔ
	status = FloatEQ(vValue2, 0.0, true, ulp) ?
		SimplexStatus::result : SimplexStatus::infeasible;
	//·´Ðý×ª¸¨Öú±äÁ¿
	lIndex = (int)(std::find(bSet.begin(), bSet.end(), -1)-bSet.begin());
	int eIndex;
	if(lIndex!=mNum) {
		eIndex = -1;
		for(int j=0; j<nNum+1; ++j) {
			if(FloatNEQ(aMatrix[lIndex][j], 0.0, true, ulp)) {
				eIndex = j;
				break;
			}
		}
		assert(eIndex!=-1);
		_SimplexAssistPivot(aMatrix, bVector, cVector2, vValue2, bSet, nSet, lIndex, eIndex);
	}
	else {
		eIndex = (int)(std::find(nSet.begin(), nSet.end(), -1)-nSet.begin());
		assert(eIndex!=nNum+1);
	}
	//½»»»¸¨Öú±äÁ¿
	if(eIndex!=nNum) {
		for(int i=0; i<mNum; ++i)
			std::swap(aMatrix[i][eIndex], aMatrix[i][nNum]);
		std::swap(nSet[eIndex], nSet[nNum]);
	}
	//È¥³ý¸¨Öú±äÁ¿
	for(int i=0; i<mNum; ++i)
		aMatrix[i].pop_back();
	nSet.pop_back();
	cVector2.assign(nNum, 0);
	for(int j=0; j<nNum; ++j) {
		if(nSet[j]<nNum)
			cVector2[j] = cVector[nSet[j]];
	}
	for(int i=0; i<mNum; ++i) {
		if(bSet[i]<nNum) {
			vValue += cVector[bSet[i]]*bVector[i];
			for(int j=0; j<nNum; ++j) {
				cVector2[j] -= cVector[bSet[i]]*aMatrix[i][j];
			}
		}
	}
	std::swap(cVector, cVector2);
	return status;
}

//µ¥´¿ÐÎ·¨£¬Ä£°åÎª¸¡µãÊý
//²ÎÊýÎª±ê×¼ÐÍ¼´£ºÏµÊý¾ØÕó¡¢³£ÊýÏòÁ¿¡¢Ä¿±êÏµÊýÏòÁ¿¡¢Ä¿±ê³£ÊýÖµ£¬¸¡µã¾ø¶ÔÎó²î½ç±¶ÂÊ
//·µ»ØÖµÎª×´Ì¬¡¢½âÏòÁ¿¡¢»ù±¾±äÁ¿ÐòºÅ¡¢·Ç»ù±¾±äÁ¿ÐòºÅ
//aMatrix¡¢bVector¡¢cVectorÖÐ¾ùÎªÎª×îÖÕËÉ³ÚÐÍµÄ½á¹¹£¬vValueÊÇÄ¿±êÖµ
//±ê×¼ÐÍµÄÑùÀý£º
//MAX: cVector^T * x + vValue
//s.t.£ºaMatrix * x <= bVector
template<typename Ty>
inline typename std::enable_if<std::is_floating_point<Ty>::value,
	std::tuple<SimplexStatus, std::vector<Ty>, std::vector<int>, std::vector<int>>
>::type SimplexAlgo(std::vector<std::vector<Ty>> &aMatrix, std::vector<Ty> &bVector,
	std::vector<Ty> &cVector, Ty &vValue, int ulp= 2000)
{
	int mNum= (int)bVector.size(), nNum= (int)cVector.size();//Ô¼ÊøÊý£¬±äÁ¿Êý
	std::vector<int> bSet, nSet;//»ù±¾¡¢·Ç»ù±¾±äÁ¿Ë÷ÒýÏÂ±ê
	std::vector<Ty> resVector;//½âÏµÊýÏòÁ¿
									  //ÕÒ³õÊ¼½â
	SimplexStatus status = _SimplexAssistInit(aMatrix, bVector, cVector, vValue, bSet, nSet, ulp);
	if(status==SimplexStatus::infeasible)
		return make_tuple(status, resVector, bSet, nSet);
	//µ¥´¿ÐÎÑ­»·
	status = _SimplexAssistLoop(aMatrix, bVector, cVector, vValue, bSet, nSet, ulp);
	if(status==SimplexStatus::unbounded)
		return make_tuple(status, resVector, bSet, nSet);
	//Éú³É½âÏòÁ¿
	resVector.resize(nNum, 0);
	for(int i=0; i<mNum; ++i) {
		if(bSet[i]<nNum)
			resVector[bSet[i]] = bVector[i];
	}
	return make_tuple(status, std::move(resVector), std::move(bSet), std::move(nSet));
}

