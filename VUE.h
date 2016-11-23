#pragma once

/*
* ǰ������
*/
class GTT_VeUE;
class RRM_VeUE;
class WT_VeUE;
class TMC_VeUE;

/*
* ÿһ����Ԫ���иõ�Ԫ��Ӧ��VeUE��ͼ
* ��������þ����ṩ��ͬ��Ԫ֮��VeUEʵ�����Ϣ����
*/

class VeUE {
	friend class System;
	/*------------------��------------------*/
private:
	/*
	* ָ��GTT��ͼ�µ�VeUEʵ�����
	*/
	GTT_VeUE * m_GTT = nullptr;

	/*
	* ָ��RRM��ͼ�µ�VeUEʵ�����
	*/
	RRM_VeUE * m_RRM = nullptr;

	/*
	* ָ��WT��ͼ�µ�VeUEʵ�����
	*/
	WT_VeUE * m_WT = nullptr;

	/*
	* ָ��TMC��ͼ�µ�VeUEʵ�����
	*/
	TMC_VeUE * m_TMC = nullptr;

	/*------------------����------------------*/
public:
	/*
	* ָ��GTT��ͼ�µ�VeUE��ָ��
	*/
	GTT_VeUE* getGTTPoint() { return m_GTT; }

	/*
	* ָ��RRM��ͼ�µ�VeUE��ָ��
	*/
	RRM_VeUE* getRRMPoint() { return m_RRM; }

	/*
	* ָ��WT��ͼ�µ�VeUE��ָ��
	*/
	WT_VeUE* getWTPoint() { return m_WT; }

	/*
	* ָ��TMC��ͼ�µ�VeUE��ָ��
	*/
	TMC_VeUE* getTMCPoint() { return m_TMC; }
};






