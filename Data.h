#pragma once


#define LIB_PATH "lib/"
#define DATA_PATH "data/"
#define CDN_FILE LIB_PATH "CdnDomainList.dat"
#define URL_FILE LIB_PATH "UrlDomainList.dat"

#define SPCDN_NAME  "spcdn"
#define OTHER_NAME "other"
#define STATIS_NAME "statis"
#define ALL_NAME "all"

#define STATIS_FILE DATA_PATH STATIS_NAME ".txt"

#define OUTPUT_INTERVAL 500000


//×Ö¶ÎÃ¶¾Ù
enum FileForm
{
	e_sip, e_dip, e_domain, e_qtype, e_qcnt,
	e_ratio, e_dir, e_auth, e_rval, e_rtype,
	e_rcode, e_ttl, e_time, e_qlen, e_rlen,
	e_rother,
	e_end,
};


//»®·Ö½á¹¹
struct Partition
{
	ofstream ofs;
	long long cnt = 0;
	void Init(const string &name, const string &strHead)
	{
		string formName = name;
		auto lbdForm = [](char ch) {
			if(IsIdChar(ch))
				return ch;
			else
				return '_';
		};
		std::transform(SHOW_BEGIN_END(formName), formName.begin(), lbdForm);
		ofs.open(name);
		ofs <<strHead;
	}
};


//¹æ·¶»¯ÓòÃû
inline bool IsDomainChar(char ch)
{
	return (ch>='0' && ch<='9')
		|| (ch>='a' && ch<='z')
		|| ch=='-' || ch=='_' || ch=='.';
}
inline bool FormDomain(string &domain)
{
	//´óÐ´×ª»»Ð¡Ð´
	std::transform(domain.begin(), domain.end(), domain.begin(), UppCharToLowChar);
	//²âÊÔÊÇ·ñÓÐÆæ¹Ö×Ö·û
	if(std::find_if_not(domain.begin(), domain.end(), IsDomainChar)!=domain.end())
		return false;
	return true;
}


//²éÕÒCDNº¯Êý
inline string *FastFindCdn(AcMachine<char, string> &mtCdn, string &domain)
{
	//´¦Àíºó×ºµã
	bool bBackDot = domain.back()=='.';
	if(bBackDot)
		domain.pop_back();
	//Ôö¼ÓÐÂºó×º
	domain.push_back('#');
	//Æ¥Åä
	auto ret = mtCdn.Judge(SHOW_BEGIN_END(domain));
	domain.pop_back();
	if(bBackDot)
		domain.push_back('.');
	return ret;
}


//Ìí¼ÓÐÅÏ¢º¯Êý
inline void AddMap(std::map<string, Partition> &mapPar, const string &strHead,
	const string &key, const string &strLine)
{
	auto res = mapPar.emplace(piecewise_construct, std::tie(key), make_tuple());
	if(res.second)
		res.first->second.Init(DATA_PATH+res.first->first, strHead);
	res.first->second.ofs <<strLine <<"\n";
	++ res.first->second.cnt;
}


//Í³¼Æº¯Êý
inline void Statistic(ofstream &ofs, std::map<string, Partition> &mapPar, long long cntValid)
{
	ofs.seekp(0);
	ofs <<std::fixed;
	ofs <<ALL_NAME <<" " <<cntValid <<" " <<"100%\n";
	for(auto &pr: mapPar) {
		ofs <<pr.first <<" " <<pr.second.cnt <<" " <<(double)pr.second.cnt*100/cntValid <<"%\n";
		pr.second.ofs <<flush;
	}
	ofs <<"end\n\n\n" <<flush;
}
