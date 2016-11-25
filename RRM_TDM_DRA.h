#pragma once
#include<vector>
#include<random>
#include<thread>
#include"RRM.h"
#include"GTT.h"
#include"Enumeration.h"
#include"Exception.h"
#include"Global.h"

//<RRM_TDM_DRA> :Radio Resource Management Time Division Dultiplexing based Distributed Resource Allocation


class RRM_TDM_DRA_VeUE:public RRM_VeUE {
	/*------------------��̬------------------*/
public:
	static std::default_random_engine s_Engine;

	/*------------------��------------------*/
public:
	/*
	* ��VeUE���ڴصĵ�ǰ����λ�õ���Ե�������
	*/
	std::tuple<int, int> m_ScheduleInterval;

	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	RRM_TDM_DRA_VeUE();

	/*
	* ���ѡȡ������Դ��
	*/
	int selectRBBasedOnP2(const std::vector<int>&t_CurAvaliablePatternIdx);

	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString(int t_NumTab);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	RRM_TDM_DRA_VeUE *const getTDM_DRAPoint()override { return this; }
	RRM_ICC_DRA_VeUE *const getICC_DRAPoint()override { throw LTEV2X_Exception("RuntimeException"); }
	RRM_RR_VeUE *const getRRPoint()override { throw LTEV2X_Exception("RuntimeException"); }
};


class RRM_TDM_DRA_RSU :public RRM_RSU{
	/*------------------��------------------*/
public:
	/*
	* TDR:Time Domain Resource
	* �±����ر��
	* tuple�洢�ı����ĺ�������Ϊ���洢ÿ����������ʱ�������������˵㣬�Ҷ˵��Լ����䳤��
	*/
	std::vector<std::tuple<int, int, int>> m_ClusterTDRInfo;

	/*
	* Pattern�����Ƿ���õı��
	* ����±����ر��
	* �ڲ��±����Pattern���
	* ��"m_PatternIsAvailable[i][j]==true"�����i��Pattern��j����
	*/
	std::vector<std::vector<bool>> m_PatternIsAvailable;

	/*
	* �����б�
	* ��ŵ���VeUEId
	* ����±����ر��
	* �ڲ�first����Emergency��second�����Emergency
	*/
	std::vector<std::pair<std::list<int>, std::list<int>>> m_AccessEventIdList;

	/*
	* �ȴ��б�
	* ��ŵ���VeUEId
	* ����±����ر��
	* �ڲ�first����Emergency��second�����Emergency
	*/
	std::vector<std::pair<std::list<int>, std::list<int>>> m_WaitEventIdList;

	/*
	* ��ŵ��ȵ�����Ϣ(�Ѿ��ɹ����룬����δ������ϣ����䴫�����֮ǰ��һֱռ�ø���Դ��)
	* ����±����ر��
	* �ڲ��±����Pattern���
	*/
	std::vector<std::vector<ScheduleInfo*>> m_ScheduleInfoTable;

	/*
	* ��ǰʱ�̵�ǰRSU�ڴ��ڴ���״̬�ĵ�����Ϣ����
	* ����±�Ϊ�ر��
	* �в��±����Pattern���
	* �ڲ���list���ڴ����ͻ������Ӧ�ض�ӦPattern�µĵ�ǰ���д�����¼��ĵ�����Ϣ
	* ���ڽ����¼������дض���Ч
	* ���ڷǽ����¼�������ǰʱ�̶�Ӧ�Ĵ���Ч
	*/
	std::vector<std::vector<std::list<ScheduleInfo*>>>  m_TransimitScheduleInfoList;

	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	RRM_TDM_DRA_RSU();

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
	* ���ص�ǰTTI���Խ����¼�����Ĵر��
	*/
	int getClusterIdx(int t_TTI);

	/*
	* ��AccessVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToAccessEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId);

	/*
	* ��WaitVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToWaitEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId);

	/*
	* ��SwitchVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToSwitchEventIdList(std::list<int>& t_SwitchVeUEIdList, int t_EventId);

	/*
	* ��TransimitScheduleInfo����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToTransimitScheduleInfoList(ScheduleInfo* t_Info);

	/*
	* ��ScheduleInfoTable����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToScheduleInfoTable(ScheduleInfo* t_Info);

	/*
	* ��RSU�����ScheduleInfoTable�ĵ�����װ���������ڲ鿴������ã����ڵ���
	*/
	void pullFromScheduleInfoTable(int t_TTI);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	RRM_TDM_DRA_RSU *const getTDM_DRAPoint()override { return this; }
	RRM_ICC_DRA_RSU *const getICC_DRAPoint() override { throw LTEV2X_Exception("RuntimeException"); }
	RRM_RR_RSU *const getRRPoint() override { throw LTEV2X_Exception("RuntimeException"); }
};


class RRM_TDM_DRA :public RRM {
	/*------------------��------------------*/
public:
	/*
	* ���ڴ�Ž���RSU�л��ĳ�������ʱ���������
	*/
	std::list<int> m_SwitchEventIdList;

	/*
	* ���ڴ��ָ������ָ��Pattern�ĸ����б�(ֻ����RSU�ڴؼ����)
	* ����±�ΪVeUEId
	* �ڲ��±�ΪPatternIdx(������)
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

	/*------------------����------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	RRM_TDM_DRA() = delete;

	/*
	* ���캯��
	* �ù��캯�������˸�ģ�����ͼ
	* ����ָ���Ա����ϵͳ���еĶ�Ӧ��Աָ�룬����ͬһʵ��
	*/
	RRM_TDM_DRA(System* t_Context);

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
	* ���ڴش�С��ʱ�ָ���
	*/
	void groupSizeBasedTDM(bool t_ClusterFlag);

	/*
	* ���ȷ����ÿ����ʱ����Դ
	*/
	void uniformTDM(bool t_ClusterFlag);

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
	* ����λ�ø���ʱ��������ȱ�
	*/
	void processScheduleInfoTableWhenLocationUpdate();

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
	* ����P1/P2/P3����Դѡ��
	*/
	void selectRBBasedOnP123();

	/*
	* ʱ��ͳ��
	*/
	void delaystatistics();

	/*
	* ֡����ͻ����ͻ�����
	*/
	void conflictListener();

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
	void writeClusterPerformInfo(std::ofstream &t_File);


	/*
	* ���ظ������������ֵ���±�
	*/
	int getMaxIndex(const std::vector<double>&t_ClusterSize);

	/*
	* ����ָ��Pattern��Ŷ�Ӧ��Pattern���ͱ�ţ����¼����ͱ��
	*/
	int getPatternType(int t_PatternIdx);

	/*
	* ����ָ��Pattern��Ŷ�Ӧ�����ز����
	*/
	std::pair<int, int> getOccupiedSubCarrierRange(MessageType t_MessageType, int t_PatternIdx);

};


inline
int RRM_TDM_DRA_VeUE::selectRBBasedOnP2(const std::vector<int>&t_CurAvaliablePatternIdx) {
	int size = static_cast<int>(t_CurAvaliablePatternIdx.size());
	if (size == 0) return -1;
	std::uniform_int_distribution<int> u(0, size - 1);
	return t_CurAvaliablePatternIdx[u(s_Engine)];
}


inline
void RRM_TDM_DRA_RSU::pushToAccessEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId) {
	if (t_IsEmergency)
		m_AccessEventIdList[t_ClusterIdx].first.push_back(t_EventId);
	else
		m_AccessEventIdList[t_ClusterIdx].second.push_back(t_EventId);
}

inline
void RRM_TDM_DRA_RSU::pushToWaitEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId) {
	if (t_IsEmergency)
		m_WaitEventIdList[t_ClusterIdx].first.push_back(t_EventId);
	else
		m_WaitEventIdList[t_ClusterIdx].second.push_back(t_EventId);
}


inline
void RRM_TDM_DRA_RSU::pushToSwitchEventIdList(std::list<int>& t_SwitchVeUEIdList, int t_EventId) {
	t_SwitchVeUEIdList.push_back(t_EventId);
}

inline
void RRM_TDM_DRA_RSU::pushToTransimitScheduleInfoList(ScheduleInfo* t_Info) {
	m_TransimitScheduleInfoList[t_Info->clusterIdx][t_Info->patternIdx].push_back(t_Info);
}


inline
void RRM_TDM_DRA_RSU::pushToScheduleInfoTable(ScheduleInfo* t_Info) {
	m_ScheduleInfoTable[t_Info->clusterIdx][t_Info->patternIdx] = t_Info;
}


inline
void RRM_TDM_DRA_RSU::pullFromScheduleInfoTable(int t_TTI) {
	/*�����ڵ��ȱ��е�ǰ���Դ������Ϣѹ��m_TransimitEventIdList*/

	/*  EMERGENCY  */
	for (int clusterIdx = 0; clusterIdx < getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
		for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx++) {
			if (m_ScheduleInfoTable[clusterIdx][patternIdx] != nullptr) {
				m_TransimitScheduleInfoList[clusterIdx][patternIdx].push_back(m_ScheduleInfoTable[clusterIdx][patternIdx]);
				m_ScheduleInfoTable[clusterIdx][patternIdx] = nullptr;
			}
		}
	}
	/*  EMERGENCY  */

	int clusterIdx = getClusterIdx(t_TTI);
	for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {
		if (m_ScheduleInfoTable[clusterIdx][patternIdx] != nullptr) {
			m_TransimitScheduleInfoList[clusterIdx][patternIdx].push_back(m_ScheduleInfoTable[clusterIdx][patternIdx]);
			m_ScheduleInfoTable[clusterIdx][patternIdx] = nullptr;
		}
	}
}

