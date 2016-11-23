#pragma once
#include<vector>
#include<list>
#include<string>
#include<utility>
#include<random>
#include"Global.h"
#include"Enumeration.h"


class Event {
	/*------------------��̬------------------*/
public:
	/*
	* �����Զ�����EventId�ľ�̬����
	*/
	static int s_EventCount;

	/*
	* ������������˱�ʱ�����������ӣ����๲��
	*/
	static std::default_random_engine s_Engine;

	/*
	* ��־��¼���ؿ���
	*/
	static bool s_LogIsOn;
	/*------------------��------------------*/
private:
	/*
	* ÿ���¼�����Ψһ��Id��Id��0��ʼ���
	*/
	const int m_EventId;

	/*
	* ���¼���Ӧ���û���Id
	*/
	const int m_VeUEId;

	/*
	* �¼�������TTIʱ��
	*/
	const int m_TriggerTTI;
	/*
	* ����Ϣ������
	*/
	const MessageType m_MessageType;

	/*
	* ��Ϣ�����ݰ�����
	*/
	const int m_PackageNum;

	/*
	* ʵ�ʴ�������ݰ��ܰ���
	* ���ڿ��ܷ���RSU�л�����˴�����ܰ������ܴ������ݰ����ܰ���
	* �ۻ�״̬��reset������
	*/
	int m_TransimitPackageNum;

	/*
	* ʵ�ʷ������������ݰ��ܰ���
	* ���ڿ��ܷ���RSU�л�����˶����ܰ������ܴ������ݰ����ܰ���
	* �ۻ�״̬��reset������
	*/
	int m_PackageLossNum;

	/*
	* ÿ������bit����
	*/
	const std::vector<int> m_BitNumPerPackage;

	/*
	* ������а��Ƿ������
	*/
	bool m_IsFinished;

	/*
	* ��Ǽ���Ҫ�����bit���ڵİ����
	*/
	int m_CurrentPackageIdx;

	/*
	* currentPackageIdx��ָ���packageʣ��������bit��
	*/
	int m_RemainBitNum;

	/*
	* ��¼ÿ�����ݰ��Ƿ񶪰�
	* һ����ֻ�ܱ���һ�Σ�����ò���ֵ����ǣ��������ö������������
	*/
	std::vector<bool> m_PackageIsLoss;

	/*
	* ��¼��������Ϻ󣬳����������޵ľ���
	* ע�⣬����Ҳ���ǰ�������ϣ�ֻ����������Ϻ��޷��������
	* ����m_PackageLossDistance
	* �ۻ�״̬��reset������
	*/
	std::vector<double> m_PackageTransimitDistance;

	/*
	* ����ʱ�������޵ľ��룬���ڷ���PDR֮��
	* ��û�з���RSU�л�ʱ�������������ᳬ���ܰ���
	* ���ǵ�����RSU�л�ʱ��������Ҫ���ã���˴�����ܰ���������ܰ���
	* ��ʱ�������ܰ���������ܰ���
	* �ۻ�״̬��reset������
	* ���Ƕ��ڸ���һ������d
	*	m_PackageLossDistance�и�ֵ���ֵĴ���Ϊn1
	*	m_PackageTransimitDistance�и�ֵ���ֵĴ���Ϊn2
	*	n1/n2���Ǹþ����µ�һ��������
	*/
	std::vector<double> m_PackageLossDistance;

	/*
	* ���˴���ʼ��С
	*/
	const int m_InitialWindowSize;

	/*
	* ���˴�����ʼ��С
	*/
	const int m_MaxWindowSize;

	/*
	* ���˴���ǰ��С
	* ��ʼ��Ϊ��ʼ����С������һ���˱ܺ��Ϊ����������󴰴�С
	*/
	int m_CurWindowSize;

	/*
	* �����˱ܶ���TTI
	*/
	int m_WithdrawalTime;

	/*
	* ��¼���¼���������־
	*/
	std::list<std::string> m_LogTrackList;

	/*
	* ��ͻ����
	*/
	int m_ConflictNum;

	/*
	* ����ʱ��
	* ���ͽڵ��ڴ�����·�Ͽ�ʼ���͵�һ��������������������һ����������Ҫ��ʱ��
	*/
	int m_SendDelay;

	/*
	* �Ŷ�ʱ��
	* �������ǰ�ȴ���ʱ��
	*/
	int m_QueueDelay;

	/*
	* ����ʱ��
	* ��һ���ڵ������˵���ýڵ���������������ʱ��
	* <<�Һ��������������>>
	* <<��ʱ���Ϊ�����ŵ���ռ�õ�ʱ���>>
	*/
	int m_ProcessDelay;

	/*------------------����------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	Event() = delete;

	/*
	* ���캯�������ܲ����ֱ�Ϊ
	*	����Id���¼������¼�����Ϣ����
	*/
	Event(int t_VeUEId, int t_TTI, MessageType t_MessageType);

	/*
	* �����¼�Id
	*/
	int getEventId() { return m_EventId; }

	/*
	* ���س���Id
	*/
	int getVeUEId() { return m_VeUEId; }

	/*
	* �����¼�����ʱ��
	*/
	int getTriggerTTI() { return m_TriggerTTI; }

	/*
	* ������Ϣ����
	*/
	MessageType getMessageType() { return m_MessageType; }

	/*
	* �����¼�����İ���Ŀ
	*/
	int getTransimitPackageNum() { return m_TransimitPackageNum; }

	/*
	* �����ܶ�����
	*/
	int getPacketLossNum() { return m_PackageLossNum; }

	/*
	* �ж��Ƿ�����¼��Ĵ���
	*/
	bool isFinished() { return m_IsFinished; }

	/*
	* ���ص�ǰ�����
	*/
	int getCurrentPackageIdx() { return m_CurrentPackageIdx; }

	/*
	* ����ʣ��bit����
	*/
	int getRemainBitNum() { return m_RemainBitNum; }

	/*
	* ���ؼ�¼���������ʱ�ľ��������������һ�������������ԭ�������
	*/
	std::vector<double> getPackageTransimitDistanceVec() { return m_PackageTransimitDistance; }

	/*
	* ���ؼ�¼����ʱ�ľ��������������һ�������������ԭ�������
	*/
	std::vector<double> getPackageLossDistanceVec() { return m_PackageLossDistance; }

	/*
	* ���س�ͻ����
	*/
	int getConflictNum() { return m_ConflictNum; }

	/*
	* ���ش���ʱ��
	*/
	int getSendDelay() { return m_SendDelay; }

	/*
	* ��������ʱ��
	*/
	void increaseSendDelay() { ++m_SendDelay; ++m_ProcessDelay; }

	/*
	* ���صȴ�ʱ��
	*/
	int getQueueDelay() { return m_QueueDelay; }

	/*
	* �����ȴ�ʱ��
	*/
	void increaseQueueDelay() { ++m_QueueDelay;  ++m_ProcessDelay; }

	/*
	* ���ش���ʱ��
	*/
	int getProcessDelay() { return m_ProcessDelay; }

	/*
	* ���·��͸���Ϣʱ��������Ϣ����״̬
	*/
	void reset();

	/*
	* ���Խ��룬������״̬
	*/
	bool tryAcccess();

	/*
	* ������Ϣ״̬
	* transimitMaxBitNumΪ���θ�ʱƵ��Դ�ɴ�������bit��
	* �����δ����ʵ��bit������С�ڸ�ֵ��������ʵ�ʴ����bit����
	*/
	int transimit(int t_TransimitMaxBitNum, double t_Distance);

	/*
	* ��ͻ֮�������Ϣ״̬
	*/
	void conflict();

	/*
	* ��ǰ����İ�����������״̬����
	*/
	void packetLoss(double t_Distance);

	/*
	* ���ɸ�ʽ�����ַ���
	*/
	std::string toString();

	/*
	* ����������־����ĸ�ʽ�����ַ���
	*/
	std::string toLogString(int t_NumTab);

	/*
	* �����־
	*/
	void addEventLog(int t_TTI, EventLogType t_EventLogType, int t_FromRSUId, int t_FromClusterIdx, int t_FromPatternIdx, int t_ToRSUId, int t_ToClusterIdx, int t_ToPatternIdx, std::string t_Description);//ѹ���µ���־
};





