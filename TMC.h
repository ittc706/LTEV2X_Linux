#pragma once
#include<list>
#include"VUE.h"
#include"RSU.h"
#include"Config.h"
#include"Event.h"

//<TMC>: Traffic Model and Control

class TMC_VeUE {
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


class TMC_RSU {
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
class TMC {
	/*------------------��------------------*/
private:
	/*
	* ָ��ϵͳ��ָ��
	*/
	System* m_Context;
public:
	/*
	* TMC��ͼ�µ�RSU����
	*/
	TMC_RSU** m_RSUAry;

	/*
	* TMC��ͼ�µ�VeUE����
	*/
	TMC_VeUE** m_VeUEAry;

	/*------------------�ӿ�------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	TMC() = delete;

	/*
	* ���캯��
	*/
	TMC(System* t_Context) : m_Context(t_Context) {}

	/*
	* ��������
	*/
	~TMC();

	/*
	* ��ȡϵͳ���ָ��
	*/
	System* getContext() { return m_Context; }

	/*
	* ��ʼ��RSU VeUE�ڸõ�Ԫ���ڲ���
	*/
	virtual void initialize() = 0;

	/*
	* �����¼�����
	*/
	virtual void buildEventList(std::ofstream& t_File)=0;

	/*
	* ���������ͳ�Ƹ�������
	*/
	virtual void processStatistics(
		std::ofstream& t_FileStatisticsDescription,
		std::ofstream& t_FileEmergencyDelay, std::ofstream& t_FilePeriodDelay, std::ofstream& t_FileDataDelay, 
		std::ofstream& t_FileEmergencyPossion, std::ofstream& t_FileDataPossion, 
		std::ofstream& t_FileEmergencyConflict, std::ofstream& t_FilePeriodConflict, std::ofstream& t_FileDataConflict,
		std::ofstream& t_FilePackageLoss, std::ofstream& t_FilePackageTransimit,
		std::ofstream& t_FileEventLog
	)=0;
};
