#pragma once

/*
* ǰ������
*/
class GTT_eNB;

/*
* ÿһ����Ԫ���иõ�Ԫ��Ӧ��eNB��ͼ
* ��������þ����ṩ��ͬ��Ԫ֮��eNBʵ�����Ϣ����
* Ŀǰ����GTT��Ԫ��Ҫ��eNB
*/

class eNB{
	friend class System;
	/*------------------��------------------*/
private:
	/*
	* ָ��GTT��ͼ�µ�eNBʵ�����
	*/
	GTT_eNB * m_GTT = nullptr;

	/*------------------����------------------*/
public:
	/*
	* ָ��GTT��ͼ�µ�eNB��ָ��
	*/
	GTT_eNB* getGTTPoint() { return m_GTT; }
};
