#pragma once
#include<vector>
#include<tuple>
#include"Enumeration.h"

//<WT>: Wireless Transmission
class VeUE;

class WT_VeUE {
	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪVeUE���ݽ�����ϵͳ��VeUE����
	*/
	VeUE* m_This;

	/*------------------����------------------*/
public:
	/*
	* ȡ��ϵͳ��System��VeUE��ָ��
	*/
	VeUE* getSystemPoint() { return m_This; }

	/*
	* ����ϵͳ��System��VeUE��ָ��
	*/
	void setSystemPoint(VeUE* t_Point) { m_This = t_Point; }
};

class RSU;

class WT_RSU {
	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪRSU���ݽ�����ϵͳ��RSU����
	*/
	RSU* m_This;

	/*------------------����------------------*/
public:
	/*
	* ȡ��ϵͳ��System��RSU��ָ��
	*/
	RSU* getSystemPoint() { return m_This; }

	/*
	* ����ϵͳ��System��RSU��ָ��
	*/
	void setSystemPoint(RSU* t_Point) { m_This = t_Point; }
};

class System;
class WT {
	/*------------------��------------------*/
private:
	friend class WT_B;
	/*
	* ָ��ϵͳ��ָ��
	*/
	System* m_Context;
public:
	/*
	* WT��ͼ�µ�RSU����
	*/
	WT_RSU** m_RSUAry;

	/*
	* WT��ͼ�µ�VeUE����
	*/
	WT_VeUE** m_VeUEAry;

	/*
	* ����SINR��ģʽ
	*/
	WTMode m_SINRMode;

	/*------------------�ӿ�------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	WT() = delete;

	/*
	* ���캯��
	* �ù��캯�������˸�ģ�����ͼ
	* ����ָ���Ա����ϵͳ���еĶ�Ӧ��Աָ�룬����ͬһʵ��
	*/
	WT(System* t_Context);

	/*
	* ��������
	*/
	~WT();

	/*
	* ��ȡϵͳ���ָ��
	*/
	System* getContext() { return m_Context; }

	/*
	* ��ʼ��RSU VeUE�ڸõ�Ԫ���ڲ���
	*/
	virtual void initialize() = 0;

	/*
	* ��ȡ��ģ���һ������
	*/
	virtual WT* getCopy() = 0;

	/*
	* �ͷŸ�ģ��Ŀ���
	*/
	virtual void freeCopy() = 0;

	/*
	* �����ظɱ�
	*/
	virtual double SINRCalculate(int t_VeUEId, int t_SubCarrierIdxStart, int t_SubCarrierIdxEnd, int t_PatternIdx) = 0;
};