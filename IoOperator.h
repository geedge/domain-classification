#pragma once

//IO²Ù×÷¿â
//20191212-1034X

#include <cstdint>
#include <cassert>

#include <iostream>
#include <fstream>

#include <limits>
#include <type_traits>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <tuple>
#include <iterator>

#include <string>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <exception>
#include <stdexcept>

#include <mutex>


//¿â¹ØÁªÐèÇóÎÄ¼þ
#include "TypeExtend.h"




//°´¸ñÊ½¶ÁÈ¡Ò»ÐÐ²¢·µ»Ø×Ö·û´®
inline bool _LineStringToInit(const std::string &strIn, std::string &strField,
	std::string &strValue)
{
	//Èô"(\t1)abc(\t2)(\t3)def(\t4)(\t5)ghi\r\n"
	//ÕÒÓÐÐ§×Ö·ûÍ·
	auto itSt = std::find_if_not(strIn.begin(), strIn.end(), IsBlankChar);//a
																								 //ÅÐ¶Ï¿Õ»ò×¢ÊÍ'#'';'"//"
	if(itSt==strIn.end() || *itSt=='#' || *itSt==';'
		||(*itSt=='/' && itSt+1!=strIn.end() && itSt[1]=='/'))
		return false;
	//ÕÒ×Ö¶ÎÎ²£¬²»ÒªÇóºóÐø
	auto itGap = std::find_if(itSt+1, strIn.end(), IsBlankChar);//\t2
	strField.assign(itSt, itGap);
	//ÕÒºóÐø´Ê
	auto itWordSt = std::find_if_not(itGap, strIn.end(), IsBlankChar);//d
	auto ritWordEd = std::find_if_not(strIn.rbegin(),
		std::string::const_reverse_iterator(itWordSt), IsBlankChar);//i
																						//´æ´¢Öµ£¬¿ÉÄÜÎª¿Õ
	if(itWordSt!=strIn.end() && itWordSt<ritWordEd.base())
		strValue.assign(itWordSt, ritWordEd.base());
	return true;
}



//ÊäÈëÁ÷¶ÁÈ¡ÅäÖÃÎÄ¼þ£¬ÓÐÔ¤¶¨Òå×Ö¶Î·¶Î§£¬¶ÁÈ¡·¶Î§ÄÚµÄÖµ£¬·µ»ØÎ´ÕÒµ½µÄµÚÒ»¸ö×Ö¶Î
//ÅäÖÃÎÄ¼þ¸ñÊ½Ó¦ÎªÈ¥µô"#define"µÄºê¶¨Òå¸ñÊ½£¬'#'';'"//"¶¼Îª×¢ÊÍ
//bReplaceÊÇ·ñÍ¬Ò»×Ö¶ÎºóÕßÌæ»»Ç°Õß£¬ÈôÎªÖØ¸´¼¯ºÏÇÒÎª·ñÔòÖØ¸´Ìí¼Ó
//bCheckNullÊÇ·ñ¾Ü¾ø¶ÁÈ¡ÖµÎª¿ÕµÄÇé¿ö
template<typename Ty1, typename Ty2>
inline Ty2 StreamReadPredefInit(std::istream &is, Ty1 &table,
	Ty2 itFieldSt, Ty2 itFieldEd, bool bReplace= true, bool bCheckNull= true)
{
	//½«¹Ø¼ü×Ö¶ÎÐ´Èë
	Ty2 itField;
	std::set<std::string> bstField;
	for(itField=itFieldSt; itField!=itFieldEd; ++itField) {
		bstField.emplace(*itField);
	}
	//´ÓÁ÷¶ÁÈ¡¹Ø¼ü×Ö¶Î
	std::string strIn, strField, strValue;
	while(std::getline(is, strIn)) {
		//½âÎöÐÐ´æÈë¶ÔÓ¦×Ö·û´®
		if(!_LineStringToInit(strIn, strField, strValue))
			continue;
		//ÅÐ¶ÏÔÚ±íÖÐ
		if(bstField.find(strField)==bstField.end())
			continue;
		//ÅÐ¶ÏÖµÎª¿Õ
		if(bCheckNull && strValue.empty())
			continue;
		//Ìæ»»Ä£Ê½ÏÂ
		if(bReplace) {
			auto itRes = table.find(strField);
			if(itRes==table.end())
				table.emplace(std::move(strField), std::move(strValue));
			else
				itRes->second = std::move(strValue);
		}
		//ÎÞ¶¯×÷Ä£Ê½ÏÂ
		else {
			table.emplace(std::move(strField), std::move(strValue));
		}
	}
	//ÅÐ¶ÏÌî³äÍê±Ï
	for(itField=itFieldSt; itField!=itFieldEd; ++itField) {
		std::string strTmp = *itField;
		auto res = table.find(strTmp);
		//ÈôÃ»ÕÒµ½×Ö¶Î»ò¼ì²é¿ÕÇÒ×Ö¶Î¿Õ
		if(res==table.end())
			break;
	}
	return itField;
}


//ÊäÈëÁ÷¶ÁÈ¡ÅäÖÃÎÄ¼þ£¬ÎÞÔ¤¶¨Òå×Ö¶Î·¶Î§£¬¶ÁÈ¡ËùÓÐÎÄ¼þµÄÖµ£¬·µ»Ø¶ÁÈ¡¸öÊý
//ÅäÖÃÎÄ¼þ¸ñÊ½Ó¦ÎªÈ¥µô"#define"µÄºê¶¨Òå¸ñÊ½£¬'#'';'"//"¶¼Îª×¢ÊÍ
//bReplaceÊÇ·ñÍ¬Ò»×Ö¶ÎºóÕßÌæ»»Ç°Õß£¬ÈôÎªÖØ¸´¼¯ºÏÇÒÎª·ñÔòÖØ¸´Ìí¼Ó
//bCheckNullÊÇ·ñ¾Ü¾ø¶ÁÈ¡ÖµÎª¿ÕµÄÇé¿ö
template<typename Ty1>
inline int StreamReadAllInit(std::istream &is, Ty1 &table,
	bool bReplace= true, bool bCheckNull= true)
{
	//´ÓÁ÷¶ÁÈ¡¹Ø¼ü×Ö¶Î
	std::string strIn, strField, strValue;
	while(std::getline(is, strIn)) {
		//½âÎöÐÐ´æÈë¶ÔÓ¦×Ö·û´®
		if(!_LineStringToInit(strIn, strField, strValue))
			continue;
		//ÅÐ¶ÏÖµÎª¿Õ
		if(bCheckNull && strValue.empty())
			continue;
		//Ìæ»»Ä£Ê½ÏÂ
		if(bReplace) {
			auto itRes = table.find(strField);
			if(itRes==table.end())
				table.emplace(std::move(strField), std::move(strValue));
			else
				itRes->second = std::move(strValue);
		}
		//ÎÞ¶¯×÷Ä£Ê½ÏÂ
		else {
			table.emplace(std::move(strField), std::move(strValue));
		}
	}
	return (int)table.size();
}



//ÊäÈëÁ÷¸¨Öúº¯Êý
//Ê¹ÓÃflushË¢ÐÂÒ»ÐÐ£¬Ê¹ÓÃendsÇå¿Õ×´Ì¬£¬Ê¹ÓÃendlÍ¬Ê±Íê³ÉË¢ÐÂºÍÇå¿Õ
inline bool operator >>(std::istream &is, 
	std::basic_ostream<char> &(*func)(std::basic_ostream<char> &))
{
	bool res = (bool)is;
	if(func==&std::flush<char, std::char_traits<char>>)
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	else if(func==&std::ends<char, std::char_traits<char>>)
		is.clear();
	else if(func==&std::endl<char, std::char_traits<char>>) {
		is.clear();
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	else
		assert(false);
	return res;
}


////»ñÈ¡ÍêÕûÐÐ×÷Îª×Ö·û´®ÊäÈëÁ÷£¬Èç¹û»ñÈ¡ÍêisÎªfailÔòÖÃ·µ»ØÁ÷Îªfail
//inline std::istringstream GetLineAsStream(std::istream &is, char gap= '\n')
//{
//	std::string strTmp;
//	std::istringstream iss;
//	if(!std::getline(is, strTmp, gap))
//		iss.setstate(std::ios_base::failbit);
//	else
//		iss.str(std::move(strTmp));
//	return std::move(iss);
//}



//Êä³öÁ÷±£³ÖÀà
//ÓÃÓÚÔÚÊä³öºó¸½¼Ó¿ØÖÆ¸ñÊ½£¬¶þ´ÎÊä³öÊ±ÕæÕýÊä³ö£¬ÐèÍâ²¿ÖØÔØÔËËã·û
template<typename Ty>
class OstreamHold
{
	OstreamHold()
		= delete;
};
//Êä³öÁ÷±£³ÖÀà×óÖµÒýÓÃÌØ»¯
template<typename Ty>
class OstreamHold<Ty &>
{
private:
	std::ostream &os;//´æ´¢os
	Ty &ref;//´æ´¢ÒýÓÃ
public:
	//¹¹Ôìº¯Êý
	explicit OstreamHold(std::ostream &o_os, Ty &o_ref):
		os(o_os), ref(o_ref)
	{
	}
	//É¾³ý¿½±´
	OstreamHold(const OstreamHold<Ty &> &)
		= delete;
	OstreamHold<Ty &> &operator =(const OstreamHold<Ty &> &)
		= delete;
	//Ä¬ÈÏÒÆ¶¯
	OstreamHold(OstreamHold<Ty &> &&)
		= default;
	OstreamHold<Ty &> &operator =(OstreamHold<Ty &> &&)
		= default;
	//Á÷ÒýÓÃ
	std::ostream &operator()() const
	{
		return os;
	}
	//ÒýÓÃºÍ³ÉÔ±ÒýÓÃ
	Ty &operator *() const
	{
		return ref;
	}
	Ty *operator ->() const
	{
		return &ref;
	}
	//×ª»»³£ÀàÐÍ°æ±¾
	operator OstreamHold<const Ty &>() const
	{
		return OstreamHold<const Ty &>(os, ref);
	}
};
//Êä³öÁ÷±£³ÖÀàÓÒÖµÒýÓÃÌØ»¯
template<typename Ty>
class OstreamHold<Ty &&>
{
private:
	std::ostream &os;//´æ´¢os
	Ty data;//´æ´¢ÒýÓÃ
public:
	//¹¹Ôìº¯Êý
	explicit OstreamHold(std::ostream &o_os, Ty &&o_data):
		os(o_os), data(std::move(o_data))
	{
	}
	//É¾³ý¿½±´
	OstreamHold(const OstreamHold<Ty &&> &)
		= delete;
	OstreamHold<Ty &&> &operator =(const OstreamHold<Ty &&> &)
		= delete;
	//Ä¬ÈÏÒÆ¶¯
	OstreamHold(OstreamHold<Ty &&> &&)
		= default;
	OstreamHold<Ty &&> &operator =(OstreamHold<Ty &&> &&)
		= default;
	//×ª»»×óÖµÁ÷
	OstreamHold<const Ty &> operator()() const
	{
		return OstreamHold<const Ty &>(os, data);
	}
	OstreamHold<Ty &> operator()()
	{
		return OstreamHold<Ty &>(os, data);
	}
	//ÒýÓÃºÍ³ÉÔ±ÒýÓÃ
	Ty &operator *() const
	{
		return data;
	}
	Ty *operator ->() const
	{
		return &data;
	}
};



//Êä³öÁ÷¼¯ºÏÎÞËø±êÖ¾£¬×÷ÎªÁ÷Êä³ö±íÊ¾ÎÞËø
class OstreamSetNoLock
{
};

//Êä³öÁ÷¼¯ºÏÀà
//ÓÃÓÚ¶à¸öÊä³öÁ÷Í¬Ê±Êä³ö£¬Ö§³Ö·ÇÁ÷ÒýÓÃ·µ»ØÖµµÄ´«µÝÁ÷²Ù×÷
//¸½¼Ó»¥³âËø¹¦ÄÜ½Ó¿Ú£¬ÒªÇó²ÎÊý¿ÉËøÀàÐÍ£¬Í¬Ê±´ËÀàÒ²Âú×ã¿ÉËø
//ÈôÓÐËøÔòÁ÷²Ù×÷×Ô¶¯£¬³ý·Ç´«µÝ·ÇËø±êÖ¾£¬ÔÚÒ»´®Á÷²Ù×÷¿ªÊ¼½áÊøÊ±½øÐÐËø¶¨½âËø
template<typename TyMtx= std::mutex>
class OstreamSet
{
public:
	//Êä³ö¼¯ºÏ´«µÝÀàÐÍ
	template<typename TyRes>
	class Hold;

public:
	std::vector<std::ostream *> vecPOs;//Êä³öÁ÷£¬¸ü¸Ä·ÇÏß³Ì°²È«
	std::vector<TyMtx *> vecPMtx;//Ëø£¬¸ü¸Ä·ÇÏß³Ì°²È«

public:
	//Ä¬ÈÏ¹¹Ôì
	OstreamSet()
		= default;
	//É¾³ý¿½±´
	OstreamSet(const OstreamSet &)
		= delete;
	OstreamSet &operator =(const OstreamSet &)
		= delete;
	//Ä¬ÈÏÒÆ¶¯£¬·ÇÏß³Ì°²È«
	OstreamSet(OstreamSet &&)
		= default;
	OstreamSet &operator =(OstreamSet &&)
		= default;
	//ÁÐ±í¹¹Ôì
	explicit OstreamSet(std::initializer_list<std::ostream *> intlOs,
		std::initializer_list<TyMtx *> intlMtx = {}
	): vecPOs(intlOs), vecPMtx(intlMtx)
	{
	}
	//µü´úÆ÷¹¹Ôì
	template<typename TyItOs, typename TyItMtx>
	explicit OstreamSet(TyItOs itOsSt, TyItOs itOsEd, 
		TyItMtx itMtxSt, TyItMtx itMtxEd)
	{
		for(; itOsSt!=itOsEd; ++itOsSt)
			vecPOs.push_back(&*itOsSt);
		for(; itMtxSt!=itMtxEd; ++itMtxSt)
			vecPMtx.push_back(&*itMtxSt);
	}
	template<typename TyItOs>
	explicit OstreamSet(TyItOs itOsSt, TyItOs itOsEd):
		OstreamSet(itOsSt, itOsEd, (TyMtx *)nullptr, (TyMtx *)nullptr)
	{
	}
public:
	//×ª·¢Á÷²Ù×÷£¬Ïß³Ì°²È«£¬°´Ë³Ðòµ÷ÓÃËø£¬Ðè±£Ö¤²»ËÀËø
	template<typename Ty>
	auto operator <<(Ty &&arg) const->
		Hold<decltype(*vecPOs.front()<<std::forward<Ty>(arg))> 
	{
		//Á÷½á¹ûÀàÐÍÊý×é
		using TyRes = decltype(*vecPOs.front()<<std::forward<Ty>(arg));
		std::vector<TypeHolder<TyRes>> vecRes;
		vecRes.reserve(vecPOs.size());
		//µ÷ÓÃËø¶¨
		Lock();
		//Êä³ö²¢´æ´¢
		for(auto pOs : vecPOs) {
			vecRes.emplace_back(*pOs <<std::forward<Ty>(arg));
		}
		//Éú³É½á¹ûµÄHoldÀàÐÍ
		return Hold<TyRes>(std::move(vecRes), vecPMtx.empty()? nullptr:this);
	}
	//¶Ô²Ù×÷·û½øÐÐ×ª·¢
	template<int c_blank =0>
	auto operator <<(std::ios_base &(*func)(std::ios_base &)) const->
		decltype(this->operator << <decltype((func))>(func))
	{
		return operator << <decltype((func))>(func);
	}
	template<int c_blank =0>
	auto operator <<(std::basic_ios<char> &(*func)(std::basic_ios<char> &)) const->
		decltype(this->operator << <decltype((func))>(func))
	{
		return operator << <decltype((func))>(func);
	}
	template<int c_blank =0>
	auto operator <<(
		std::basic_ostream<char> &(*func)(std::basic_ostream<char> &)) const->
		decltype(this->operator << <decltype((func))>(func))
	{
		return operator << <decltype((func))>(func);
	}
	//¶Ô²»Ëø±êÖ¾½øÐÐÁ÷²Ù×÷£¬Á÷Êä³ö½«²»½øÐÐËø¶¨ºÍ½âËø
	Hold<std::ostream &> operator <<(OstreamSetNoLock) const
	{
		//Á÷½á¹ûÀàÐÍÊý×é
		using TyRes = std::ostream &;
		std::vector<TypeHolder<TyRes>> vecRes;
		vecRes.reserve(vecPOs.size());
		//´æ´¢ÀàÐÍ
		for(auto pOs : vecPOs) {
			vecRes.emplace_back(*pOs);
		}
		//Éú³É½á¹ûµÄHoldÀàÐÍ
		return Hold<TyRes>(std::move(vecRes), nullptr);
	}
	//µ÷ÓÃostreamµÄ³ÉÔ±º¯Êý£¬½øÐÐËø¶¨£¬ºöÂÔ·µ»ØÖµ
	template<typename Ty, typename ...Tys>
	void operator()(const Ty &pmf, Tys &&...args) const
	{
		//µ÷ÓÃËø¶¨
		Lock();
		//µ÷ÓÃ³ÉÔ±º¯Êý
		for(auto pOs : vecPOs) {
			(pOs->*pmf)(std::forward<Tys>(args)...);
		}
		//µ÷ÓÃ½âËø
		Unlock();
	}
	//µ÷ÓÃËø¶¨
	void Lock() const
	{
		//Ë³Ðòµ÷ÓÃËø
		for(auto pMtx : vecPMtx)
			pMtx->lock();
	}
	void lock() const
	{
		Lock();
	}
	//µ÷ÓÃ½âËø
	void Unlock() const
	{
		//·´Ðò½âËø
		for(auto rit =vecPMtx.rbegin(); rit!=vecPMtx.rend(); ++rit)
			(*rit)->unlock();
	}
	void unlock() const
	{
		Unlock();
	}
	//³¢ÊÔËø¶¨
	bool TryLock() const
	{
		//Ë³Ðòµ÷ÓÃ³¢ÊÔËø
		unsigned i;
		for(i =0; i<vecPMtx.size(); ++i) {
			if(!vecPMtx[i]->try_lock())
				break;
		}
		//³¢ÊÔÊ§°ÜÔò½âËø
		if(i!=vecPMtx.size()) {
			for(--i; i>=0; --i)
				vecPMtx[i]->unlock();
			return false;
		}
		else
			return true;
	}
	bool try_lock() const
	{
		return TryLock();
	}
};

//Êä³öÁ÷¼¯ºÏ´«µÝÀàÐÍ
template<typename TyMtx>
template<typename TyRes>
class OstreamSet<TyMtx>::Hold
{
	friend class OstreamSet<TyMtx>;
private:
	std::vector<TypeHolder<TyRes>> vecRes;//Êä³öÁ÷µÄ½á¹ûÊý×é
	const OstreamSet<TyMtx> *pSet;//Êä³öÁ÷¼¯ºÏÖ¸Õë£¬Îö¹¹½âËø±êÖ¾

private:
	//¹¹Ôì£¬Êä³ö¼¯ºÏÒýÓÃ³õÊ¼»¯
	explicit Hold(std::vector<TypeHolder<TyRes>> &&o_vecRes,
		const OstreamSet<TyMtx> *o_pSet
	): vecRes(std::move(o_vecRes)), pSet(o_pSet)
	{
	}
	//É¾³ý¿½±´
	Hold(const Hold &)
		= delete;
	Hold &operator =(const Hold &)
		= delete;
	//ÒÆ¶¯²Ù×÷£¬ÒÆ³öÊý¾Ý²»ÐèÎö¹¹½âËø±êÖ¾
	Hold(Hold &&other) noexcept
	{
		this->operator =(std::move(other));
	}
	Hold &operator =(Hold &&other) noexcept
	{
		if(this!=&other) {
			vecRes = std::move(other.vecRes);
			pSet = std::move(other.pSet);
			other.pSet = nullptr;
		}
		return *this;
	}
public:
	//Îö¹¹º¯Êý£¬¸ù¾Ý±êÖ¾½øÐÐ½âËø£¬·´Ðò½âËø
	~Hold()
	{
		if(pSet!=nullptr) {
			//µ÷ÓÃ½âËø
			pSet->Unlock();
		}
	}
public:
	//×ª·¢Á÷²Ù×÷£¬×ª·¢ºóÈ¥³ý½âËø±êÖ¾
	template<typename Ty>
	auto operator <<(Ty &&arg)->
		Hold<decltype(vecRes.front().hold<<std::forward<Ty>(arg))>
	{
		//´Ë¶ÔÏó²»ÐèÎö¹¹½âËø
		auto ptr = pSet;
		pSet = nullptr;
		//ÐÂµÄÁ÷²Ù×÷½á¹ûÀàÐÍ
		using TyResTmp = decltype(vecRes.front().hold<<std::forward<Ty>(arg));
		std::vector<TypeHolder<TyResTmp>> vecResTmp;
		vecResTmp.reserve(vecRes.size());
		//Á÷Êä³ö²¢´æ´¢
		for(auto &refRes : vecRes) {
			vecResTmp.emplace_back(refRes.hold <<std::forward<Ty>(arg));
		}
		//¹¹ÔìÐÂµÄHoldÀàÐÍ£¬×ªÒåÎö¹¹½âËø±êÖ¾
		return Hold<TyResTmp>(std::move(vecResTmp), ptr);
	}
	//¶Ô²Ù×÷·û½øÐÐ×ª·¢
	template<int c_blank =0>
	auto operator <<(std::ios_base &(*func)(std::ios_base &))->
		decltype(this->operator << <decltype((func))>(func))
	{
		return operator << <decltype((func))>(func);
	}
	template<int c_blank =0>
	auto operator <<(std::basic_ios<char> &(*func)(std::basic_ios<char> &))->
		decltype(this->operator << <decltype((func))>(func))
	{
		return operator << <decltype((func))>(func);
	}
	template<int c_blank =0>
	auto operator <<(
		std::basic_ostream<char> &(*func)(std::basic_ostream<char> &))->
		decltype(this->operator << <decltype((func))>(func))
	{
		return operator << <decltype((func))>(func);
	}
};



//¶ÏÑÔ²Ù×÷ÀàÊä³öÑ¡Ïî
enum class AssertOption
{
	thrw_log,//Å×³öÒì³£²¢Êä³öÈÕÖ¾
	thrw,//Å×³öÒì³£
	asst_log,//µ÷ÊÔ¶ÏÑÔ²¢Êä³öÈÕÖ¾
	asst,//µ÷ÊÔ¶ÏÑÔ
	log,//Êä³öÈÕÖ¾
	none,//ÎÞ²Ù×÷
};

//¶ÏÑÔ²Ù×÷Àà
template<typename TyOs= std::ostream>
class AssertOperator
{
private:
	//·Ç²¶»ñÒì³£
	class NoCatchError:
		private std::runtime_error
	{
		//¼Ì³Ð»ùÀà¹¹Ôìº¯Êý£¨³ýÄ¬ÈÏ¿½±´¹¹Ôì£©
		using std::runtime_error::runtime_error;
	public:
		//Ä¬ÈÏ¹¹Ôì
		NoCatchError():
			std::runtime_error("")
		{
		}
		//Ê¹ÓÃ¼Ì³Ðwhat
		using std::runtime_error::what;
		//Ê¡ÂÔºÏ³É5¿½±´
	};

private:
	TyOs &os;//Êä³öÀà
public:
	AssertOption option;//Êä³öÑ¡Ïî

public:
	//¹¹Ôì
	AssertOperator() =
		delete;
	explicit AssertOperator(TyOs &o_os, AssertOption o_option):
		os(o_os), option(o_option)
	{
	}
public:
	//Ö¸¶¨Ñ¡Ïî½øÐÐ¶ÏÑÔ
	bool operator()(AssertOption optionTmp,
		bool bCond, const std::string &str= "") const
	{
		if(bCond)
			return true;
		//°´Ñ¡Ïî½øÐÐ²Ù×÷
		if(optionTmp==AssertOption::thrw_log) {
			os <<str <<std::flush;
			throw NoCatchError(str);
		}
		else if(optionTmp==AssertOption::thrw) {
			throw NoCatchError(str);
		}
		else if(optionTmp==AssertOption::asst_log) {
			os <<str <<std::flush;
			assert(false);
		}
		else if(optionTmp==AssertOption::asst) {
			assert(false);
		}
		else if(optionTmp==AssertOption::log) {
			os <<str <<std::flush;
		}
		else if(optionTmp==AssertOption::none) {
		}
		return false;
	}
	//Ê¹ÓÃÄ¬ÈÏÑ¡Ïî½øÐÐ¶ÏÑÔ
	bool operator()(bool bCond, const std::string &str= "") const
	{
		return operator()(option, bCond, str);
	}
};



//Ð´ÀàÐÍ¶þ½øÖÆÎÄ¼þÀà
//²»Í¬»·¾³µÄÄÚÖÃÀàÐÍ´óÐ¡²»Ò»¶¨ÏàÍ¬£¬Òª±£Ö¤Ê¹ÓÃ´óÐ¡ÎÞ¹Ø±äÁ¿
class BinWriteFile
{
public:
	std::ofstream ofs;//ÎÄ¼þÁ÷

public:
	//¹¹Ôì
	BinWriteFile()
		= default;
	explicit BinWriteFile(const std::string &name, bool o_bAfter=false)
	{
		Open(name, o_bAfter);
	}
	//Îö¹¹
	~BinWriteFile()
		= default;
	//É¾³ý¿½±´
	BinWriteFile(const BinWriteFile &)
		= delete;
	BinWriteFile &operator =(BinWriteFile &)
		= delete;
	//ÒÆ¶¯¹¹Ôì
	BinWriteFile(BinWriteFile &&file)
		= default;
	//ÒÆ¶¯¸³Öµ
	BinWriteFile &operator =(BinWriteFile &&file)
		= default;

public:
	//´ò¿ªÎÄ¼þ£¬o_bAfterÊÇ·ñ×·¼ÓÐ´
	BinWriteFile &Open(const std::string &name, bool o_bAfter=false)
	{
		ofs.close();
		if(o_bAfter)
			ofs.open(name, std::ios::binary|std::ios::app);
		else
			ofs.open(name, std::ios::binary);
		if(!ofs.is_open())
			throw std::runtime_error("Can't Open Write File");
		return *this;
	}
	//¹Ø±ÕÎÄ¼þ
	bool Close()
	{
		ofs.close();
		if(ofs.bad())
			throw std::runtime_error("Can't Close File");
		return true;
	}
	//·µ»Ø´ò¿ª×´Ì¬
	bool isopen() const
	{
		return ofs.is_open();
	}
	//ÎÄ¼þÁ÷¿ÉÓÃ£¬Ò»¶¨¿ÉÓÃ
	bool IsFail() const
	{
		return !ofs.good();
	}
	operator bool() const
	{
		return ofs.good();
	}
	//Çå³ý´íÎó±êÖ¾
	void Clear()
	{
		ofs.clear();
	}
	//Ë¢ÐÂÎÄ¼þ»º³åÇø
	BinWriteFile &Fflush()
	{
		ofs.flush();
		return *this;
	}
	//ÖØ¶¨Î»Î»ÖÃ
	BinWriteFile &Seekp(std::ofstream::pos_type pos)
	{
		ofs.seekp(pos);
		return *this;
	}
	BinWriteFile &Seekp(std::ofstream::off_type off, std::ios_base::seekdir dir)
	{
		ofs.seekp(off, dir);
		return *this;
	}
	//²éÑ¯Î»ÖÃ
	std::ofstream::pos_type Tellp()
	{
		return ofs.tellp();
	}
	//¶Ô»º³åÇø½øÐÐÎÄ¼þÐ´Èë
	BinWriteFile &WriteBuf(const void *buf, size_t size)
	{
		ofs.write(reinterpret_cast<const char *>(buf), size);
		if(ofs.bad())
			throw std::runtime_error("Can't Write File");
		return *this;
	}
	//¶ÔÄÚÖÃÀàÐÍÊý×é½øÐÐ¶ÁÈ¡
	template<typename Ty>
	typename std::enable_if<std::is_scalar<Ty>::value, BinWriteFile &
	>::Type WriteArray(const Ty *pt, size_t count)
	{
		WriteBuf(pt, sizeof(Ty)*count);
		return *this;
	}
	//¶Ô·ÇÄÚÖÃÀàÐÍÊý×é½øÐÐ¶ÁÈ¡
	template<typename Ty>
	typename std::enable_if<!std::is_scalar<Ty>::value, BinWriteFile &
	>::Type WriteArray(const Ty *pt, size_t count)
	{
		for(; count!=0; --count, ++pt)
			*this <<*pt;
		return *this;
	}
public:
	//¶ÔÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þÐ´Èë£¬°üÀ¨ÄÚÖÃÀàÐÍÊý×é
	template<typename Ty>
	typename std::enable_if<std::is_scalar<
		typename std::remove_all_extents<Ty>::type>::value, BinWriteFile &
	>::type operator <<(const Ty &r)
	{
		WriteBuf(&r, sizeof(r));
		return *this;
	}
	//¶Ô·ÇÄÚÖÃÀàÐÍÊý×é½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty, size_t N>
	typename std::enable_if<!std::is_scalar<
		typename std::remove_all_extents<Ty>::type>::value, BinWriteFile &
	>::type operator <<(const Ty (&arrStr)[N])
	{
		for(int64_t i=0; i<N; ++i)
			*this <<arrStr[i];
		return *this;
	}
	//¶Ôpair½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty1, typename Ty2>
	BinWriteFile &operator <<(const std::pair<Ty1, Ty2> &pr)
	{
		*this <<pr.first <<pr.second;
		return *this;
	}
	//¶Ôtuple½øÐÐÎÄ¼þÐ´Èë
	template<typename ...Tys>
	BinWriteFile &operator <<(const std::tuple<Tys...> &tup)
	{
		TupleWrite<sizeof...(Tys)-1>(tup);
		return *this;
	}
	template<int index, typename ...Tys>
	typename std::enable_if<(index>=0), void
	>::type TupleWrite(const std::tuple<Tys...> &tup)
	{
		*this <<std::get<index>(tup);
		TupleWrite<index-1>(tup);
	}
	template<int index, typename Ty>
	void TupleWrite(const Ty &tup)
	{
	}
	//¶Ôstring½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty>
	BinWriteFile &operator <<(const std::basic_string<Ty> &str)
	{
		//Ð´³ß´ç
		int64_t size = str.size();
		operator <<(size);
		//Ð´Êý¾Ý
		WriteBuf(&str[0], (size_t)(size*sizeof(Ty)));
		return *this;
	}
	//¶ÔvectorÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty>
	typename std::enable_if<std::is_scalar<Ty>::value, BinWriteFile &
	>::type operator <<(const std::vector<Ty> &vec)
	{
		//Ð´³ß´ç
		int64_t size = vec.size();
		operator <<(size);
		//Ð´Êý¾Ý
		WriteBuf(vec.data(), size*sizeof(Ty));
		return *this;
	}
	//¶Ôvector·ÇÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty>
	typename std::enable_if<!std::is_scalar<Ty>::value, BinWriteFile &
	>::type operator <<(const std::vector<Ty> &vec)
	{
		//Ð´³ß´ç
		int64_t size = vec.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : vec)
			*this <<r;
		return *this;
	}
	//¶ÔarrayÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty, size_t c_size>
	typename std::enable_if<std::is_scalar<Ty>::value, BinWriteFile &
	>::type operator <<(const std::array<Ty, c_size> &arr)
	{
		//Ð´Êý¾Ý
		WriteBuf(arr.data(), c_size*sizeof(Ty));
		return *this;
	}
	//¶Ôarray·ÇÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty, size_t c_size>
	typename std::enable_if<!std::is_scalar<Ty>::value, BinWriteFile &
	>::type operator <<(const std::array<Ty, c_size> &arr)
	{
		//Ð´Êý¾Ý
		for(auto &r : arr)
			*this <<r;
		return *this;
	}
	//¶Ôdeque½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty>
	BinWriteFile &operator <<(const std::deque<Ty> &deq)
	{
		//Ð´³ß´ç
		int64_t size = deq.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : deq)
			*this <<r;
		return *this;
	}
	//¶Ôlist½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty>
	BinWriteFile &operator <<(const std::list<Ty> &lis)
	{
		//Ð´³ß´ç
		int64_t size = lis.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : lis)
			*this <<r;
		return *this;
	}
	//¶Ô¶þ²æÊ÷½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty, typename TyComp>
	BinWriteFile &operator <<(const std::set<Ty, TyComp> &bst)
	{
		//Ð´³ß´ç
		int64_t size = bst.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : bst)
			*this <<r;
		return *this;
	}
	//¶Ô¶àÖµ¶þ²æÊ÷½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty, typename TyComp>
	BinWriteFile &operator <<(const std::multiset<Ty, TyComp> &bst)
	{
		//Ð´³ß´ç
		int64_t size = bst.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : bst)
			*this <<r;
		return *this;
	}
	//¶Ô¶þ²æÊ÷¶Ô½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty1, typename Ty2, typename TyComp>
	BinWriteFile &operator <<(const std::map<Ty1, Ty2, TyComp> &bst)
	{
		//Ð´³ß´ç
		int64_t size = bst.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : bst)
			*this <<r.first <<r.second;
		return *this;
	}
	//¶Ô¶àÖµ¶þ²æÊ÷¶Ô½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty1, typename Ty2, typename TyComp>
	BinWriteFile &operator <<(const std::multimap<Ty1, Ty2, TyComp> &bst)
	{
		//Ð´³ß´ç
		int64_t size = bst.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : bst)
			*this <<r.first <<r.second;
		return *this;
	}
	//¶Ô¹þÏ£±í½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty, typename TyHash, typename TyComp>
	BinWriteFile &operator <<(const std::unordered_set<Ty, TyHash, TyComp> &htb)
	{
		//Ð´³ß´ç
		int64_t size = htb.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : htb)
			*this <<r;
		return *this;
	}
	//¶Ô¶àÖµ¹þÏ£±í½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty, typename TyHash, typename TyComp>
	BinWriteFile &operator <<(const std::unordered_multiset<Ty, TyHash, TyComp> &htb)
	{
		//Ð´³ß´ç
		int64_t size = htb.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : htb)
			*this <<r;
		return *this;
	}
	//¶Ô¹þÏ£±í¶Ô½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty1, typename Ty2, typename TyHash, typename TyComp>
	BinWriteFile &operator <<(const std::unordered_map<Ty1, Ty2, TyHash, TyComp> &htb)
	{
		//Ð´³ß´ç
		int64_t size = htb.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : htb)
			*this <<r.first <<r.second;
		return *this;
	}
	//¶Ô¶àÖµ¹þÏ£±í¶Ô½øÐÐÎÄ¼þÐ´Èë
	template<typename Ty1, typename Ty2, typename TyHash, typename TyComp>
	BinWriteFile &operator <<(const std::unordered_multimap<Ty1, Ty2, TyHash, TyComp> &htb)
	{
		//Ð´³ß´ç
		int64_t size = htb.size();
		operator <<(size);
		//Ð´Êý¾Ý
		for(auto &r : htb)
			*this <<r.first <<r.second;
		return *this;
	}
};

//Ð´ÀàÐÍ¶þ½øÖÆÎÄ¼þÀàµü´úÆ÷
//ÊÇÊä³öµü´úÆ÷
template<typename Ty>
class BinWriteFileIter:
	public std::iterator<std::output_iterator_tag, void,
	void, void, void>
{
private://Êý¾Ý
	BinWriteFile *pBwf;//¹ØÁªÎÄ¼þ

public://»ù±¾º¯Êý
		 //Ã»ÓÐÄ¬ÈÏ¹¹Ôì
	BinWriteFileIter()
		= delete;
	//¹ØÁª¹¹Ôì
	BinWriteFileIter(BinWriteFile &bwf):
		pBwf(&bwf)
	{
	}
public://ÔËËã·ûÖØÔØ
		 //½âÒýÓÃ²Ù×÷£¬·µ»Ø×ÔÉí
	BinWriteFileIter &operator *() const
	{
		return *this;
	}
	//µÝÔö²Ù×÷£¬Ê²Ã´Ò²²»×ö£¬·µ»Ø×ÔÉí
	BinWriteFileIter &operator ++()
	{
		return *this;
	}
	BinWriteFileIter &operator ++(int)
	{
		return *this;
	}
	//¸³Öµ²Ù×÷£¬½øÐÐÊä³ö
	BinWriteFileIter &operator =(const Ty &data)
	{
		*pBwf <<data;
		return *this;
	}
};


template<typename Ty>
class BinReadFileIter;
//¶ÁÀàÐÍ¶þ½øÖÆÎÄ¼þÀà
//²»Í¬»·¾³µÄÄÚÖÃÀàÐÍ´óÐ¡²»Ò»¶¨ÏàÍ¬£¬Òª±£Ö¤Ê¹ÓÃ´óÐ¡ÎÞ¹Ø±äÁ¿
class BinReadFile
{
public:
	std::ifstream ifs;//ÎÄ¼þÁ÷
	bool bFast = false;//¿ìËÙÄ£Ê½£¬£¬Ò»´ÎÉêÇë×ã¹»ÄÚ´æ£¬Êý×é¼ÇÂ¼´íÎó¿ÉÄÜÔì³ÉÄÚ´æ²»×ã£¬¿ÉÔÚÈÎºÎÊ±ºò¸ü¸Ä
	bool bSave = false;//½ÚÔ¼Ä£Ê½£¬ÉêÇëÄÚ´æÊ±²»ÉêÇë¶îÍâÁ¿£¬½ö¿ìËÙÄ£Ê½ÓÃ£¬¿ÉÔÚÈÎºÎÊ±ºò¸ü¸Ä
	static constexpr int64_t bufSize = 4096;//¶ÁÈ¡»º´æ´óÐ¡

public:
	//¹¹Ôì
	explicit BinReadFile(bool o_bFast= false, bool o_bSave= false):
		bFast(o_bFast), bSave(o_bSave)
	{
	}
	//´ò¿ªÎÄ¼þ¹¹Ôì
	template<typename Ty, typename= typename std::enable_if<
		IsRemoveCVRefSameSzOrString<Ty>::value>::type
	> explicit BinReadFile(Ty &&name, bool o_bFast= false, bool o_bSave= false):
		bFast(o_bFast), bSave(o_bSave)
	{
		Open(std::forward<Ty>(name));
	}
	//Îö¹¹
	~BinReadFile()
		= default;
	//É¾³ý¿½±´
	BinReadFile(const BinReadFile &)
		= delete;
	BinReadFile &operator =(BinReadFile &)
		= delete;
	//ÒÆ¶¯¹¹Ôì
	BinReadFile(BinReadFile &&file)
		= default;
	//ÒÆ¶¯¸³Öµ
	BinReadFile &operator =(BinReadFile &&file)
		= default;

public:
	//´ò¿ªÎÄ¼þ
	BinReadFile &Open(const std::string &name)
	{
		ifs.close();
		ifs.open(name, std::ios::binary);
		if(ifs.bad())
			throw std::runtime_error("Open Read File Bad Error");
		return *this;
	}
	//¸²¸ÇCloseº¯Êý£¬Èô¼ì²â½áÎ²Ôò·µ»Ø¼ì²â½á¹û
	bool Close(bool bTestEnd = false)
	{
		bool res = !bTestEnd;
		//¼ì²â½áÎ²
		if(bTestEnd) {
			res = TestEof();
		}
		ifs.close();
		if(ifs.bad())
			throw std::runtime_error("Can't Close File");
		return res;
	}
	//·µ»Ø´ò¿ª×´Ì¬
	bool IsOpen() const
	{
		return ifs.is_open();
	}
	//ÎÄ¼þÁ÷¿ÉÓÃ
	bool IsFail() const
	{
		return !ifs.good();
	}
	operator bool() const
	{
		return ifs.good();
	}
	//Çå³ý´íÎó±êÖ¾
	void Clear()
	{
		ifs.clear();
	}
	//¼ì²â½áÎ²£¬»á½øÐÐÒ»´Î¶ÁÈ¡²Ù×÷
	bool TestEof()
	{
		ifs.get();
		if(ifs.bad())
			throw std::runtime_error("Read File Bad Error");
		return ifs.eof();
	}
	//ÖØ¶¨Î»Î»ÖÃ
	BinReadFile &Seekg(std::ifstream::pos_type pos)
	{
		ifs.seekg(pos);
		return *this;
	}
	BinReadFile &Seekg(std::ifstream::off_type off, std::ios_base::seekdir dir)
	{
		ifs.seekg(off, dir);
		return *this;
	}
	//²éÑ¯Î»ÖÃ
	std::ifstream::pos_type Tellg()
	{
		return ifs.tellg();
	}
	//¶Ô»º³åÇø½øÐÐÎÄ¼þ¶ÁÈ¡
	BinReadFile &ReadBuf(void *buf, size_t size)
	{
		ifs.read(reinterpret_cast<char *>(buf), size);
		if(ifs.bad())
			throw std::runtime_error("Read File Bad Error");
		return *this;
	}
	//¶ÔÄÚÖÃÀàÐÍÊý×é½øÐÐ¶ÁÈ¡
	template<typename Ty>
	typename std::enable_if<std::is_scalar<Ty>::value, BinReadFile &
	>::Type ReadArray(Ty *pt, size_t count)
	{
		ReadBuf(pt, sizeof(Ty)*count);
		return *this;
	}
	//¶Ô·ÇÄÚÖÃÀàÐÍÊý×é½øÐÐ¶ÁÈ¡
	template<typename Ty>
	typename std::enable_if<!std::is_scalar<Ty>::value, BinReadFile &
	>::Type ReadArray(Ty *pt, size_t count)
	{
		for(; count!=0; --count, ++pt) {
			if(ifs.fail())
				return;
			*this >>*pt;
		}
		return *this;
	}
public:
	//¶ÔÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þ¶ÁÈ¡£¬°üÀ¨ÄÚÖÃÀàÐÍÊý×é
	template<typename Ty>
	typename std::enable_if<std::is_scalar<
		typename std::remove_all_extents<Ty>::type>::value, BinReadFile &
	>::type operator >>(Ty &r)
	{
		ReadBuf(&r, sizeof(r));
		return *this;
	}
	//¶Ô·ÇÄÚÖÃÀàÐÍÊý×é½øÐÐÎÄ¼þ¶ÁÈ¡
	template<typename Ty, size_t N>
	typename std::enable_if<!std::is_scalar<
		typename std::remove_all_extents<Ty>::type>::value, BinReadFile &
	>::type operator >>(Ty (&arrStr)[N])
	{
		for(int64_t i=0; i<N; ++i) {
			if(ifs.fail())
				return *this;
			*this >>arrStr[i];
		}
		return *this;
	}
	//¶Ôpair½øÐÐÎÄ¼þ¶ÁÈ¡
	template<typename Ty1, typename Ty2>
	BinReadFile &operator >>(std::pair<Ty1, Ty2> &pr)
	{
		*this >>pr.first >>pr.second;
		return *this;
	}
	//¶Ôtuple½øÐÐÎÄ¼þ¶ÁÈ¡
	template<typename ...Tys>
	BinReadFile &operator >>(std::tuple<Tys...> &tup)
	{
		TupleRead<sizeof...(Tys)-1>(tup);
		return *this;
	}
	template<int index, typename ...Tys>
	typename std::enable_if<(index>=0), void
	>::type TupleRead(std::tuple<Tys...> &tup)
	{
		*this >>std::get<index>(tup);
		TupleRead<index-1>(tup);
	}
	template<int index, typename Ty>
	void TupleRead(Ty &tup)
	{
	}
	//¶Ôstring½øÐÐÎÄ¼þ¶ÁÈ¡
	template<typename Ty>
	BinReadFile &operator >>(std::basic_string<Ty> &str)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		str.clear();
		//¶ÁÊý¾Ý
		if(bFast) {
			if(bSave)
				str.reserve((typename std::basic_string<Ty>::size_type)size);
			str.resize((typename std::basic_string<Ty>::size_type)size);
			ReadBuf(&str[0], (size_t)(size*sizeof(Ty)));
		}
		else {
			constexpr int64_t perBufSize = bufSize/sizeof(Ty);
			for(; size>0; size-=perBufSize) {
				if(ifs.fail())
					return *this;
				int64_t realSize = size<perBufSize? size:perBufSize;
				auto nowSize = str.size();
				str.resize((typename std::basic_string<Ty>::size_type)(nowSize+realSize));
				ReadBuf(&str[0]+nowSize, (size_t)(realSize*sizeof(Ty)));
			}
		}
		return *this;
	}
	//¶ÔvectorÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þ¶ÁÈ¡
	template<typename Ty>
	typename std::enable_if<std::is_scalar<Ty>::value, BinReadFile &
	>::type operator >>(std::vector<Ty> &vec)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		constexpr int64_t perBufSize = bufSize/sizeof(Ty);
		vec.clear();
		//¶ÁÊý¾Ý
		if(bFast) {
			if(bSave)
				vec.reserve((typename std::vector<Ty>::size_type)size);
			vec.resize((typename std::vector<Ty>::size_type)size);
			ReadBuf(vec.data(), size*sizeof(Ty));
		}
		else {
			for(; size>0; size-=perBufSize) {
				if(ifs.fail())
					return *this;
				int64_t realSize = size<perBufSize? size:perBufSize;
				auto nowSize = vec.size();
				vec.resize((typename std::vector<Ty>::size_type)(nowSize+realSize));
				ReadBuf(vec.data()+nowSize, realSize*sizeof(Ty));
			}
		}
		return *this;
	}
	//¶Ôvector·ÇÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹Ôìº¯Êý
	template<typename Ty>
	typename std::enable_if<!std::is_scalar<Ty>::value, BinReadFile &
	>::type operator >>(std::vector<Ty> &vec)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		//¶ÁÊý¾Ý
		vec.clear();
		if(bFast) {
			if(bSave)
				vec.reserve((typename std::vector<Ty>::size_type)size);
			vec.resize((typename std::vector<Ty>::size_type)size);
			for(auto &r : vec) {
				if(ifs.fail())
					return *this;
				*this >>r;
			}
		}
		else {
			for(int64_t i=0; i<size; ++i) {
				if(ifs.fail())
					return *this;
				vec.emplace_back();
				*this >>vec.back();
			}
		}
		return *this;
	}
	//¶ÔarrayÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þ¶ÁÈ¡
	template<typename Ty, size_t c_size>
	typename std::enable_if<std::is_scalar<Ty>::value, BinReadFile &
	>::type operator >>(std::array<Ty, c_size> &arr)
	{
		//¶ÁÊý¾Ý
		ReadBuf(arr.data(), c_size*sizeof(Ty));
		return *this;
	}
	//¶Ôarray·ÇÄÚÖÃÀàÐÍ½øÐÐÎÄ¼þ¶ÁÈ¡
	template<typename Ty, size_t c_size>
	typename std::enable_if<!std::is_scalar<Ty>::value, BinReadFile &
	>::type operator >>(std::array<Ty, c_size> &arr)
	{
		//¶ÁÊý¾Ý
		for(auto &ele: arr) {
			if(ifs.fail())
				return *this;
			*this >>ele;
		}
		return *this;
	}
	//¶Ôdeque½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹Ôìº¯Êý
	template<typename Ty>
	BinReadFile &operator >>(std::deque<Ty> &deq)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		deq.clear();
		//¶ÁÊý¾Ý
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			deq.emplace_back();
			*this >>deq.back();
		}
		return *this;
	}
	//¶Ôlist½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹Ôìº¯Êý
	template<typename Ty>
	BinReadFile &operator >>(std::list<Ty> &lis)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		lis.clear();
		//¶ÁÊý¾Ý
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			lis.emplace_back();
			*this >>lis.back();
		}
		return *this;
	}
	//¶Ô¶þ²æÊ÷½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty, typename TyComp>
	BinReadFile &operator >>(std::set<Ty, TyComp> &bst)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		bst.clear();
		//¶ÁÊý¾Ý
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty data;
			*this >>data;
			bst.insert(bst.end(), std::move(data));
		}
		return *this;
	}
	//¶Ô¶àÖµ¶þ²æÊ÷½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty, typename TyComp>
	BinReadFile &operator >>(std::multiset<Ty, TyComp> &bst)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		bst.clear();
		//¶ÁÊý¾Ý
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty data;
			*this >>data;
			bst.insert(bst.end(), std::move(data));
		}
		return *this;
	}
	//¶Ô¶þ²æÊ÷¶Ô½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty1, typename Ty2, typename TyComp>
	BinReadFile &operator >>(std::map<Ty1, Ty2, TyComp> &bst)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		bst.clear();
		//¶ÁÊý¾Ý
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty1 data;
			*this >>data;
			if(ifs.fail())
				return *this;
			auto res = bst.emplace_hint(bst.end(), std::piecewise_construct,
				std::forward_as_tuple(std::move(data)), std::make_tuple());
			*this >>res->second;
		}
		return *this;
	}
	//¶Ô¶àÖµ¶þ²æÊ÷¶Ô½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty1, typename Ty2, typename TyComp>
	BinReadFile &operator >>(std::multimap<Ty1, Ty2, TyComp> &bst)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		bst.clear();
		//¶ÁÊý¾Ý
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty1 data;
			*this >>data;
			if(ifs.fail())
				return *this;
			auto res = bst.emplace_hint(bst.end(), std::piecewise_construct,
				std::forward_as_tuple(std::move(data)), std::make_tuple());
			*this >>res->second;
		}
		return *this;
	}
	//¶Ô¹þÏ£±í½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty, typename TyHash, typename TyComp>
	BinReadFile &operator >>(std::unordered_set<Ty, TyHash, TyComp> &htb)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		htb.clear();
		//¶ÁÊý¾Ý
		if(bFast)
			htb.reserve((typename std::unordered_set<Ty, TyHash, TyComp>::size_type)size);
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty data;
			*this >>data;
			htb.insert(std::move(data));
		}
		return *this;
	}
	//¶Ô¶àÖµ¹þÏ£±í½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty, typename TyHash, typename TyComp>
	BinReadFile &operator >>(std::unordered_multiset<Ty, TyHash, TyComp> &htb)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		htb.clear();
		//¶ÁÊý¾Ý
		if(bFast)
			htb.reserve((typename std::unordered_multiset<Ty, TyHash, TyComp>::size_type)size);
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty data;
			*this >>data;
			htb.insert(std::move(data));
		}
		return *this;
	}
	//¶Ô¹þÏ£±í¶Ô½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty1, typename Ty2, typename TyHash, typename TyComp>
	BinReadFile &operator >>(std::unordered_map<Ty1, Ty2, TyHash, TyComp> &htb)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		htb.clear();
		//¶ÁÊý¾Ý
		if(bFast)
			htb.reserve((typename std::unordered_map<Ty1, Ty2, TyHash, TyComp>::size_type)size);
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty1 data;
			*this >>data;
			if(ifs.fail())
				return *this;
			auto res = htb.emplace(std::piecewise_construct,
				std::forward_as_tuple(std::move(data)), std::make_tuple()).first;
			*this >>res->second;
		}
		return *this;
	}
	//¶Ô¶àÖµ¹þÏ£±í¶Ô½øÐÐÎÄ¼þ¶ÁÈ¡£¬ÐèÄ¬ÈÏ¹¹ÔìºÍÒÆ¶¯¸³Öµ
	template<typename Ty1, typename Ty2, typename TyHash, typename TyComp>
	BinReadFile &operator >>(std::unordered_multimap<Ty1, Ty2, TyHash, TyComp> &htb)
	{
		//¶Á³ß´ç
		int64_t size = -1;
		operator >>(size);
		if(ifs.fail())
			return *this;
		htb.clear();
		//¶ÁÊý¾Ý
		if(bFast)
			htb.reserve((typename std::unordered_multimap<Ty1, Ty2, TyHash, TyComp>::size_type)size);
		for(int64_t i=0; i<size; ++i) {
			if(ifs.fail())
				return *this;
			Ty1 data;
			*this >>data;
			if(ifs.fail())
				return *this;
			auto res = htb.emplace(std::piecewise_construct,
				std::forward_as_tuple(std::move(data)), std::make_tuple());
			*this >>res->second;
		}
		return *this;
	}
};

//¶ÁÀàÐÍ¶þ½øÖÆÎÄ¼þÀàµü´úÆ÷
//ÊÇÊäÈëµü´úÆ÷£¬´æ´¢ÀàÐÍ±ØÐë¿ÉÄ¬ÈÏ¹¹Ôì
template<typename Ty>
class BinReadFileIter:
	public std::iterator<std::input_iterator_tag, Ty,
	int64_t, Ty *, Ty &>
{
private://Êý¾Ý
	BinReadFile *pBrf;//¹ØÁªÎÄ¼þ
	int64_t size;//Ê£ÓàÐ´Èë´óÐ¡£¬¸ºÊý±íÊ¾ÎÞÏÞÐ´Èë
	Ty data;//´æ´¢Êý¾Ý

public://»ù±¾º¯Êý
		 //Ä¬ÈÏ¹¹Ôì£¬³ÉÎªÎ²ºóµü´úÆ÷
	BinReadFileIter():
		pBrf(nullptr), size(0)
	{
	}
	//¹ØÁª¹¹Ôì£¬³õÊ¼½øÐÐ¶ÁÈ¡£¬¿ÉÑ¡ÔñµÄ¼ÓÈë¶ÁÈ¡ÊýÁ¿ÏÞÖÆ
	BinReadFileIter(BinReadFile &brf, int64_t o_size= -1):
		pBrf(&brf), size(o_size)
	{
		Read();
	}
public://ÔËËã·ûÖØÔØ
		 //½âÒýÓÃ²Ù×÷£¬·µ»ØÊý¾ÝÒýÓÃ£¬Ö§³ÖÒÆ¶¯²Ù×÷
	Ty &operator *() const
	{
		return data;
	}
	Ty *operator ->() const
	{
		return &operator *();
	}
	//µÝÔö²Ù×÷£¬Í¬Ê±½øÐÐ¶ÁÈ¡
	BinReadFileIter &operator ++()
	{
		Read();
		return *this;
	}
	BinReadFileIter operator ++(int)
	{
		BinReadFileIter ret(*this);
		operator ++();
		return ret;
	}
	//±È½Ï²Ù×÷£¬Ö¸ÏòÏàÍ¬µÄÎÄ¼þ¾ÍÈÏÎªÏàÍ¬
	bool operator ==(const BinReadFileIter &other) const
	{
		return pBrf==other.pBrf;
	}
	bool operator !=(const BinReadFileIter &other) const
	{
		return !operator ==(other);
	}
private:
	//¶ÁÈ¡Êý¾Ý£¬Ã¿´Î¶ÁÈ¡ÊÇÎö¹¹ÔÙÄ¬ÈÏ¹¹ÔìÒ»´ÎÊý¾Ý£¬ÒÔÖ§³ÖÒÆ¶¯²Ù×÷
	void Read()
	{
		assert(pBrf!=nullptr);
		//ÈôÊ£Óà0Ôò×ªÎªÎ²ºó
		if(size==0) {
			pBrf = nullptr;
			return;
		}
		//Çå³ý
		CallDestruct(data);
		new(&data) Ty();
		//¶ÁÈ¡
		*pBrf >>data;
		//Èô¶ÁÈ¡Ê§°ÜÔò×ªÎªÎ²ºó
		if(!*pBrf)
			pBrf = nullptr;
		//¼õÉÙÊ£Óà
		if(size>0)
			-- size;
	}
};
