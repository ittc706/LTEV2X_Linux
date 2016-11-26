#pragma once
#include<list>
#include<vector>

//<TMC>: Traffic Model and Control
class VeUE;

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

class RSU;

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
	/*------------------��̬------------------*/
public:
	/*
	* �����¼�/�����¼�/����ҵ���¼� �����ݰ�����
	* �±���MessageType�Ķ���Ϊ׼
	*/
	static const std::vector<int> s_MESSAGE_PACKAGE_NUM;

	/*
	* �����¼�/�����¼�/����ҵ���¼� ÿ�����ݰ���bit����
	* �±���MessageType�Ķ���Ϊ׼
	*/
	static const std::vector<std::vector<int>> s_MESSAGE_BIT_NUM_PER_PACKAGE;

	/*
	* �����¼�/�����¼�/����ҵ���¼� ��ʼ���˱ܴ���С
	* �±���MessageType�Ķ���Ϊ׼
	*/
	static const std::vector<int> s_INITIAL_WINDOW_SIZE;

	/*
	* �����¼�/�����¼�/����ҵ���¼� �����˱ܴ���С
	* �±���MessageType�Ķ���Ϊ׼
	*/
	static const std::vector<int> s_MAX_WINDOW_SIZE;
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
