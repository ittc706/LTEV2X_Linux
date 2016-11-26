#pragma once
#include<vector>
#include<list>
#include"Enumeration.h"

//<RRM>: Radio Resource Management

class VeUE;
class RRM_TDM_DRA_VeUE;
class RRM_ICC_DRA_VeUE;
class RRM_RR_VeUE;

class RRM_VeUE {
	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪVeUE���ݽ�����ϵͳ��VeUE����
	*/
	VeUE* m_This;

public:
	/*
	* �±��Ӧ��Pattern�£�ͬƵ��������
	*/
	std::vector<int> m_InterferenceVeUENum;

	/*
	* �±��Ӧ��Pattern�£�ͬƵ���ų���ID
	* ��������ǰ����
	*/
	std::vector<std::vector<int>> m_InterferenceVeUEIdVec;

	/*
	* ����ͬ�ϣ���һ�εĸ��ų���
	* �����ж��Ƿ���ͬ���Ӷ������Ƿ���Ҫ����һ���ŵ��Լ��ظɱ�
	*/
	std::vector<std::vector<int>> m_PreInterferenceVeUEIdVec;

	/*
	* ���Ʒ�ʽ
	*/
	const ModulationType m_ModulationType;

	/*
	* �ŵ���������
	*/
	const double m_CodeRate;

	/*
	* �ϴμ�����ظɱ�
	* ��Ϊ��Сֵ((std::numeric_limits<double>::min)())��˵��֮ǰû�м����
	*/
	std::vector<double> m_PreSINR;

	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	RRM_VeUE() = delete;
	RRM_VeUE(int t_TotalPatternNum);

	/*
	* ��������
	* û����Ҫ�������Դ
	*/
	~RRM_VeUE(){}

	/*
	* �ж��Ƿ���Ҫ���¼���SINR
	* ȡ���ڸ����б��Ƿ���ͬ
	*/
	bool isNeedRecalculateSINR(int t_PatternIdx);

	/*
	* �ж�֮ǰ�Ƿ��Ѿ����SINR
	*/
	bool isAlreadyCalculateSINR(int t_PatternIdx) { return m_PreSINR[t_PatternIdx] != (std::numeric_limits<double>::min)(); }

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	* ���ھ�̬����ΪRRM_VeUE
	*/
	virtual RRM_TDM_DRA_VeUE *const getTDM_DRAPoint() = 0;
	virtual RRM_ICC_DRA_VeUE *const getICC_DRAPoint() = 0;
	virtual RRM_RR_VeUE *const getRRPoint() = 0;

	/*
	* ȡ��ϵͳ��System��VeUE��ָ��
	*/
	VeUE* getSystemPoint();

	/*
	* ����ϵͳ��System��VeUE��ָ��
	*/
	void setSystemPoint(VeUE* t_Point);
};

class RSU;
class RRM_TDM_DRA_RSU;
class RRM_ICC_DRA_RSU;
class RRM_RR_RSU;

class RRM_RSU {
public:
	/*------------------�ڲ���------------------*/
	class ScheduleInfo {
		/*--------��--------*/
	public:
		/*
		* �¼����
		*/
		int eventId;

		/*
		* �������
		*/
		int VeUEId;

		/*
		* RSU���
		*/
		int RSUId;

		/*
		* �ر��
		*/
		int clusterIdx;

		/*
		* Ƶ�����
		*/
		int patternIdx;

		/*
		* ��ǰ��������ݰ��ı��
		* ֱ�Ӵ���Ϣ���ȡ
		*/
		int currentPackageIdx = -1;

		/*
		* ʣ�����bit����(����ʵ�ʴ����bit���������ǵ�Ч����ʵ���ݵĴ�����������Ҫ��ȥ�ŵ����������bit)
		* ֱ�Ӵ���Ϣ���ȡ
		*/
		int remainBitNum = -1;

		/*
		* ��ǰ�����bit����(����ʵ�ʴ����bit���������ǵ�Ч����ʵ���ݵĴ�����������Ҫ��ȥ�ŵ����������bit)
		* ֱ�Ӵ���Ϣ���ȡ
		*/
		int transimitBitNum = -1;

		/*-------����-------*/
		/*
		* ���캯��
		*/
		ScheduleInfo() {}

		/*
		* ���캯��
		*/
		ScheduleInfo(int t_EventId, int t_VeUEId, int t_RSUId, int t_ClusterIdx, int t_PatternIdx) {
			this->eventId = t_EventId;
			this->VeUEId = t_VeUEId;
			this->RSUId = t_RSUId;
			this->clusterIdx = t_ClusterIdx;
			this->patternIdx = t_PatternIdx;
		}

		/*
		* ���ɸ�ʽ���ַ���
		*/
		std::string toLogString();

		/*
		* ���ɱ�ʾ������Ϣ��string����
		* �����¼���Id��������Id���Լ�Ҫ������¼���ռ�õ�TTI����
		*/
		std::string toScheduleString(int t_NumTab);
	};

	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪRSU���ݽ�����ϵͳ��RSU����
	*/
	RSU* m_This;

	/*------------------����------------------*/
public:
	/*
	* ��ʼ��
	* ���ֳ�Ա��Ҫ�ȵ�GTTģ���ʼ����Ϻ����˴ص��������ܽ��б���ԪRSU�ĳ�ʼ��
	*/
	virtual void initialize() = 0;

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	* ���ھ�̬����ΪRRM_RSU
	*/
	virtual RRM_TDM_DRA_RSU *const getTDM_DRAPoint() = 0;
	virtual RRM_ICC_DRA_RSU *const getICC_DRAPoint() = 0;
	virtual RRM_RR_RSU *const getRRPoint() = 0;

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
class RRM {
	/*------------------��̬------------------*/
public:
	/*
	* 10MHz���ܴ���(Hz)
	*/
	static const int s_TOTAL_BANDWIDTH = 10 * 1000 * 1000;

	/*
	* ÿ��RB�Ĵ���(Hz)
	*/
	static const int s_BANDWIDTH_OF_RB = 12 * 1000 * 15;

	/*
	* ��λ(��),����RB����Ϊ180kHz��TTIΪ1ms����˵�λTTI��λRB����ı�����Ϊ180k*1ms=180
	*/
	static const int s_BIT_NUM_PER_RB = 180;

	/*
	* ���Ʒ�ʽ
	*/
	static const ModulationType s_MODULATION_TYPE = QPSK;

	/*
	* �ŵ���������
	*/
	static const double s_CODE_RATE;

	/*
	* �������������С�ظɱ�
	* �����ж��Ƿ񶪰�֮��
	*/
	static const double s_DROP_SINR_BOUNDARY;
	/*------------------��------------------*/
private:
	/*
	* ָ��ϵͳ��ָ��
	*/
	System* m_Context;
public:
	/*
	* RRM��ͼ�µ�RSU����
	*/
	RRM_RSU** m_RSUAry;

	/*
	* RRM��ͼ�µ�VeUE����
	*/
	RRM_VeUE** m_VeUEAry;
													  
	/*
	* ����ģ�����ʱ���¼
	*/
	long double m_GTTTimeConsume = 0;
	long double m_WTTimeConsume = 0;

	/*------------------�ӿ�------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	RRM() = delete;

	/*
	* ���캯��
	*/
	RRM(System* t_Context) : m_Context(t_Context) {}

	/*
	* ��������
	*/
	~RRM();

	/*
	* ��ȡϵͳ���ָ��
	*/
	System* getContext() { return m_Context; }

	/*
	* ��ʼ��RSU VeUE�ڸõ�Ԫ���ڲ���
	*/
	virtual void initialize() = 0;

	/*
	* ������λ�ø���ʱ���������ĵ��������Ϣ
	*/
	virtual void cleanWhenLocationUpdate() = 0;

	/*
	* �����ܿغ���
	*/
	virtual void schedule() = 0;
};


