ï»¿// DomainDeal.cpp : æ­¤æä»¶åå« "main" å½æ°ãç¨åºæ§è¡å°å¨æ­¤å¤å¼å§å¹¶ç»æã
//

#include "Common.h"
#include "Data.h"


AssertOperator<> g_asst(cout, AssertOption::thrw_log);

int main(int argc, char *argv[])
{
	
	//å¤æ­åæ°
	if(argc<2) {
		cout <<"input filename as arg" <<endl;
		return -1;
	}

	//è¯»åæä»¶
	ifstream ifs(argv[1]);
	if(!ifs.is_open()) {
		cout <<"cant open file" <<endl;
		return -1;
	}

	cout <<"read metadata start\n";
	//è¯»ååæ°æ®
	AcMachine<char, string> mtCdn;
	TrieTree<char, string> mtUrl;
	BinReadFile brf(true, true);
	g_asst(brf.Open(CDN_FILE)
		&& (brf >>mtCdn)
		&& brf.Close(true),
		"cant read cdn data\n");
	g_asst(brf.Open(URL_FILE)
		&& (brf >>mtUrl)
		&& brf.Close(true),
		"cant read url data\n");
	cout <<"read metadata success\n";

	//ç»ç»è¾åºç»æ
	std::map<string, Partition> mapPar;
	ofstream ofs(STATIS_FILE);

	//æé¤è¡
	cout <<"deal dns start\n";
	string strHead;
	long long cntLine= 0, cntForm= 0, cntValid= 0;
	constexpr long long limLine = -1;
	constexpr bool bOutHead = false;
	for(int i=0; i!=1; ++i) {
		string str;
		std::getline(ifs, str);
		if(bOutHead)
			strHead <<str <<"\n";
		++ cntLine;
	}
	//æè¡è¯»å
	for(string strLine; std::getline(ifs, strLine)
		&& (limLine<=0 || cntLine<limLine); ++cntLine)
	{
		//é´éè¾åºä¸­é´ç»æ
		if(cntLine%OUTPUT_INTERVAL==0) {
			Statistic(ofs, mapPar, cntValid);
			cout <<"\r" <<cntLine <<" lines" <<flush;
		}
		//è§£æå­æ®µ
		istringstream iss(strLine);
		vector<string> vec;
		for(int i=0; i!=e_end; ++i) {
			vec.emplace_back();
			if(!std::getline(iss, vec.back(), '\t')) {
				break;
			}
		}
		if(vec.size()!=e_end)
			continue;
		++ cntForm;
		//å¤çåå
		if(!FormDomain(vec[e_domain]))
			continue;
		++ cntValid;
		//æ¥æ¾CDN
		string &strOut = vec[e_domain];
		string *res;
		if((res= mtUrl.Judge(vec[e_domain].rbegin(), vec[e_domain].rend())))
			AddMap(mapPar, strHead, *res, strOut);
		else if((res= FastFindCdn(mtCdn, vec[e_domain])))
			AddMap(mapPar, strHead, SPCDN_NAME, strOut);
		else
			AddMap(mapPar, strHead, OTHER_NAME, strOut);
	}
	cout <<"\ndeal dns success\n";
	cout <<"cntLine: " <<cntLine <<"\n"
		<<"cntForm: " <<cntForm <<"\n"
		<<"cntValid: " <<cntValid <<"\n";

	//æç»ç»è®¡
	Statistic(ofs, mapPar, cntValid);
}

