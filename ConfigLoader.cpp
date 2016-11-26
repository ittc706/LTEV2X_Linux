/*
* =====================================================================================
*
*       Filename:  ConfigLoader.cpp
*
*    Description:  TMC模块
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<iostream>
#include<stdexcept>
#include"ConfigLoader.h"

using namespace std;

void ConfigLoader::resolvConfigPath(string t_FilePath) {
	//清空缓存
	m_Content.clear();
	m_TagContentMap.clear();

	//读取待解析字符串
	ifstream in(t_FilePath);
	istream_iterator<char> if_it(in), if_eof;
	m_Content.assign(if_it, if_eof);
	in.close();

	//解析，并存储
	regex r("<([^<>]*)>([^<>]*)</([^<>]*)>");
	for (sregex_iterator it(m_Content.begin(), m_Content.end(), r), eof; it != eof; ++it) {
		string leftTag = it->operator[](1);
		string rightTag = it->operator[](3);
		string content = it->operator[](2);
		if (leftTag != rightTag) {
			throw logic_error("tag not match");
		}
		m_TagContentMap[leftTag] = content;
	}
}

std::string ConfigLoader::getParam(std::string t_Param) {
	if (m_TagContentMap.find(t_Param) == m_TagContentMap.end()) return "";
	return m_TagContentMap[t_Param];
}