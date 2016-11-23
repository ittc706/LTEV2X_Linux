#pragma once

/*
* ǰ������
*/
class GTT_RSU;
class RRM_RSU;
class WT_RSU;
class TMC_RSU;

/*
* ÿһ����Ԫ���иõ�Ԫ��Ӧ��RSU��ͼ
* ��������þ����ṩ��ͬ��Ԫ֮��RSUʵ�����Ϣ����
*/

class RSU {
	friend class System;
	/*------------------��------------------*/
private:
	/*
	* ָ��GTT��ͼ�µ�RSUʵ�����
	*/
	GTT_RSU * m_GTT = nullptr;

	/*
	* ָ��RRM��ͼ�µ�RSUʵ�����
	*/
	RRM_RSU * m_RRM = nullptr;

	/*
	* ָ��WT��ͼ�µ�RSUʵ�����
	*/
	WT_RSU * m_WT = nullptr;

	/*
	* ָ��TMC��ͼ�µ�RSUʵ�����
	*/
	TMC_RSU * m_TMC = nullptr;

	/*------------------����------------------*/
public:
	/*
	* ָ��GTT��ͼ�µ�RSU��ָ��
	*/
	GTT_RSU* getGTTPoint() { return m_GTT; }

	/*
	* ָ��RRM��ͼ�µ�RSU��ָ��
	*/
	RRM_RSU* getRRMPoint() { return m_RRM; }

	/*
	* ָ��WT��ͼ�µ�RSU��ָ��
	*/
	WT_RSU* getWTPoint() { return m_WT; }

	/*
	* ָ��TMC��ͼ�µ�RSU��ָ��
	*/
	TMC_RSU* getTMCPoint() { return m_TMC; }
};




