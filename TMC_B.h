#pragma once
#include<vector>
#include"TMC.h"

//<TMC_B>: Traffic Model and Control Base

class TMC_B :public TMC {
	/*------------------��------------------*/
public:
	/*
	* ÿ���������¼������Ĵ���
	* ������֤���ɷֲ��������в����ô�
	*/
	std::vector<int> m_VeUEEmergencyNum;

	/*
	* ÿ��������ҵ���¼������Ĵ���
	* ������֤���ɷֲ��������в����ô�
	*/
	std::vector<int> m_VeUEDataNum;

	/*
	* ÿ���¼��ɹ��������Ŀ
	* ����±�Ϊ�¼�����
	*/
	std::vector<int> m_TransimitSucceedEventNumPerEventType;

	/*------------------����------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	TMC_B() = delete;

	/*
	* ���캯��
	*/
	TMC_B(System* t_Context);

	/*
	* ��ʼ��RSU VeUE�ڸõ�Ԫ���ڲ���
	*/
	void initialize()override;

	/*
	* �����¼�����
	*/
	void buildEventList(std::ofstream& t_File) override;

	/*
	* ���������ͳ�Ƹ�������
	*/
	void processStatistics(
		std::ofstream& t_FileStatisticsDescription,
		std::ofstream& t_FileEmergencyDelay, std::ofstream& t_FilePeriodDelay, std::ofstream& t_FileDataDelay,
		std::ofstream& t_FileEmergencyPossion, std::ofstream& t_FileDataPossion,
		std::ofstream& t_FileEmergencyConflict, std::ofstream& t_FilePeriodConflict, std::ofstream& t_FileDataConflict,
		std::ofstream& t_FilePackageLoss, std::ofstream& t_FilePackageTransimit,
		std::ofstream& t_FileEventLog
	) override;

private:
	/*
	* д���¼��б����Ϣ
	*/
	void writeEventListInfo(std::ofstream &t_File);

	/*
	* д�����¼�����־��Ϣ
	*/
	void writeEventLogInfo(std::ofstream &t_File);
};