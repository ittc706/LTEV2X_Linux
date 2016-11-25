#pragma once
#include<vector>
#include<list>
#include<thread>
#include"RRM.h"
#include"Exception.h"
#include"Global.h"

//<RRM_RR>: Radio Resource Management Round-Robin


class RRM_RR_VeUE :public RRM_VeUE {
	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	RRM_RR_VeUE();

	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString(int t_NumTab);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	RRM_TDM_DRA_VeUE *const getTDM_DRAPoint()override { throw LTEV2X_Exception("RuntimeException"); }
	RRM_ICC_DRA_VeUE *const getICC_DRAPoint()override { throw LTEV2X_Exception("RuntimeException"); }
	RRM_RR_VeUE *const getRRPoint()override { return this; }
};


class RRM_RR_RSU :public RRM_RSU {
	/*------------------��------------------*/
public:
	/*
	* RSU����Ľ����б�
	* ����±�Ϊ�ر��
	* �ڲ�list��ŵ��Ǵ��ڵȴ�����״̬��VeUEId
	*/
	std::vector<std::list<int>> m_AccessEventIdList;

	/*
	* RSU����ĵȴ��б�
	* ����±�Ϊ�ر��
	* �ڲ�list��ŵ��Ǵ��ڵȴ�����״̬��VeUEId
	* ����Դ�У�
	*		1���ִغ���System�����л�����ת���RSU����ĵȴ�����
	*		2���¼������е�ǰ���¼�������ת��ȴ�����
	*/
	std::vector<std::list<int>> m_WaitEventIdList;

	/*
	* ��ŵ��ȵ�����Ϣ�����ν��д���
	* ����±����ر��
	* �ڲ��±����Pattern���
	*/
	std::vector<std::vector<ScheduleInfo*>> m_TransimitScheduleInfoTable;

	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	RRM_RR_RSU();

	/*
	* ��ʼ��
	* ���ֳ�Ա��Ҫ�ȵ�GTTģ���ʼ����Ϻ����˴ص��������ܽ��б���ԪRSU�ĳ�ʼ��
	*/
	void initialize() override;

	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString(int t_NumTab);

	/*
	* ��AccessVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToAccessEventIdList(int t_ClusterIdx, int t_EventId);

	/*
	* ��WaitVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToWaitEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId);

	/*
	* ��SwitchVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToSwitchEventIdList(int t_EventId, std::list<int>& t_SwitchVeUEIdList);

	/*
	* ��ScheduleInfoTable����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToTransimitScheduleInfoTable(ScheduleInfo* t_Info);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	RRM_TDM_DRA_RSU *const getTDM_DRAPoint()override { throw LTEV2X_Exception("RuntimeException"); }
	RRM_ICC_DRA_RSU *const getICC_DRAPoint() override { throw LTEV2X_Exception("RuntimeException"); }
	RRM_RR_RSU *const getRRPoint() override { return this; }
};


class RRM_RR :public RRM {
	/*------------------��------------------*/
public:
	/*
	* ���ڴ�Ž���RSU�л��ĳ�������ʱ���������
	*/
	std::list<int> m_SwitchEventIdList;

	/*
	* ���ڴ��ָ������ָ��Pattern�ĸ����б�(ֻ����RSU�ڴؼ����)
	* ����±�ΪVeUEId
	* �ڲ��±�ΪPatternIdx
	*/
	std::vector<std::vector<std::list<int>>> m_InterferenceVec;

	/*
	* ���߳�����
	*/
	int m_ThreadNum;

	/*
	* ���߳�����
	*/
	std::vector<std::thread> m_Threads;

	/*
	* �����ÿ���̵߳�RSUId��Χ
	*/
	std::vector<std::pair<int, int>> m_ThreadsRSUIdRange;
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	RRM_RR() = delete;

	/*
	* ���캯��
	* �ù��캯�������˸�ģ�����ͼ
	* ����ָ���Ա����ϵͳ���еĶ�Ӧ��Աָ�룬����ͬһʵ��
	*/
	RRM_RR(System* t_Context);

	/*
	* ��ʼ��RSU VeUE�ڸõ�Ԫ���ڲ���
	*/
	void initialize() override;

	/*
	* ������λ�ø���ʱ���������ĵ��������Ϣ
	*/
	void cleanWhenLocationUpdate()override;

	/*
	* RRM_ICC_DRA�����ܿأ����ǻ�����麯��
	*/
	void schedule() override;

private:
	/*
	* ��Դ������Ϣ���
	*/
	void informationClean();

	/*
	* ���µȴ�����
	*/
	void updateAccessEventIdList(bool t_ClusterFlag);

	/*
	* �����¼�����
	* ����ʱ�̴������¼�����RSU�ĵȴ�������
	*/
	void processEventList();

	/*
	* ����λ�ø���ʱ������ȴ�����
	*/
	void processWaitEventIdListWhenLocationUpdate();

	/*
	* ����λ�ø���ʱ������ת�ӱ�
	*/
	void processSwitchListWhenLocationUpdate();

	/*
	* ����ȴ��������ɽ�������
	*/
	void processWaitEventIdList();

	/*
	* ��ѯ���ȣ����䵱ǰTTI����Դ(���Ǹ���ScheduleTable)
	*/
	void roundRobin();

	/*
	* ʱ��ͳ��
	*/
	void delaystatistics();

	/*
	* ��������ŵ���Ӧ
	*/
	void transimitPreparation();

	/*
	* ģ�⴫�俪ʼ�����µ�����Ϣ���ۼ�������
	*/
	void transimitStart();
	void transimitStartThread(int t_FromRSUId, int t_ToRSUId);

	/*
	* ģ�⴫�����������״̬
	*/
	void transimitEnd();

	/*
	* ��¼������Ϣ��־
	*/
	void writeScheduleInfo(std::ofstream& t_File);

	/*
	* ��TTIΪ������¼��־
	*/
	void writeTTILogInfo(std::ofstream& t_File, int t_TTI, EventLogType t_EventLogType, int t_EventId, int t_FromRSUId, int t_FromClusterIdx, int t_FromPatternIdx, int t_ToRSUId, int t_ToClusterIdx, int t_ToPatternIdx, std::string t_Description);

	/*
	* д��ִ���Ϣ����־
	*/
	void writeClusterPerformInfo(bool isLocationUpdate, std::ofstream& t_File);

	/*
	* ����ָ��Pattern��Ŷ�Ӧ�����ز����
	*/
	std::pair<int, int> getOccupiedSubCarrierRange(int t_PatternIdx);
};



inline
void RRM_RR_RSU::pushToAccessEventIdList(int t_ClusterIdx, int t_EventId) {
	m_AccessEventIdList[t_ClusterIdx].push_back(t_EventId);
}

inline
void RRM_RR_RSU::pushToWaitEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId) {
	if (t_IsEmergency)
		m_WaitEventIdList[t_ClusterIdx].insert(m_WaitEventIdList[t_ClusterIdx].begin(), t_EventId);
	else
		m_WaitEventIdList[t_ClusterIdx].push_back(t_EventId);
}


inline
void RRM_RR_RSU::pushToSwitchEventIdList(int t_EventId, std::list<int>& t_SwitchVeUEIdList) {
	t_SwitchVeUEIdList.push_back(t_EventId);
}


inline
void RRM_RR_RSU::pushToTransimitScheduleInfoTable(ScheduleInfo* t_Info) {
	m_TransimitScheduleInfoTable[t_Info->clusterIdx][t_Info->patternIdx] = t_Info;
}