#pragma once

/*
* ǰ������
*/
class GTT_Road;

/*
* ÿһ����Ԫ���иõ�Ԫ��Ӧ��Road��ͼ
* ��������þ����ṩ��ͬ��Ԫ֮��Roadʵ�����Ϣ����
* Ŀǰ����GTT��Ԫ��Ҫ��Road
*/

class Road{
	friend class System;
	/*------------------��------------------*/
private:
	/*
	* ָ��GTT��ͼ�µ�Roadʵ�����
	*/
	GTT_Road * m_GTT = nullptr;

	/*------------------����------------------*/
public:
	/*
	* ����ָ��GTT��ͼ�µ�Road��ָ��
	*/
	GTT_Road* getGTTPoint() { return m_GTT; }
};