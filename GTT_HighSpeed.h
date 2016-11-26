#pragma once
#include<stdexcept>
#include"GTT.h"

// <GTT_HighSpeed>: Geographical Topology and Transport HighSpeed


class GTT_HighSpeed_VeUE:public GTT_VeUE {
	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	GTT_HighSpeed_VeUE() = delete;
	GTT_HighSpeed_VeUE(VeUEConfig &t_VeUEConfig);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_VeUE  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_VeUE  *const getHighSpeedPoint()override { return this; }
};


class  GTT_HighSpeed_RSU :public GTT_RSU {
public:
	/*
	* ���캯��
	*/
	GTT_HighSpeed_RSU();

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_RSU  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_RSU  *const getHighSpeedPoint()override { return this; }
};


class  GTT_HighSpeed_eNB :public GTT_eNB {
public:
	/*
	* ��ʼ������
	* ���ù��캯����ԭ���ǹ����ʱ���������û�������
	*/
	void initialize(eNBConfig &t_eNBConfig)override;

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_eNB  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_eNB  *const getHighSpeedPoint()override { return this; }
};

 
class GTT_HighSpeed_Road :public GTT_Road {
public:
	/*
	* ���캯��
	*/
	GTT_HighSpeed_Road(HighSpeedRodeConfig &t_RoadHighSpeedConfig);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_Road  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_Road  *const getHighSpeedPoint()override { return this; }
};


class GTT_HighSpeed :public GTT {
	/*------------------��̬------------------*/
public:
	/*
	* ��������棬���๲��
	*/
	static std::default_random_engine s_Engine;

	/*
	* ��վ����
	*/
	static const int s_eNB_NUM = 2;

	/*
	* ��·����
	*/
	static const int s_ROAD_NUM = 6;

	/*
	* RSU����
	*/
	static const int s_RSU_NUM = 35;

	/*
	* RSU�ڴ�����
	*/
	static const int s_RSU_CLUSTER_NUM = 2;

	/*
	* ·��
	*/
	static const int s_ROAD_LENGTH = 3464;

	/*
	* ·��
	*/
	static const double s_ROAD_WIDTH;

	/*
	* ???
	*/
	static const double s_ISD;

	/*
	* ???
	*/
	static const double s_ROAD_TOPO_RATIO[s_ROAD_NUM * 2];

	/*
	* ???
	*/
	static const double s_RSU_TOPO_RATIO[s_RSU_NUM * 2];

	/*
	* ???
	*/
	static const double s_eNB_TOPO[s_eNB_NUM * 2];
	/*------------------��------------------*/
private:
	/*
	* ����Rode����
	*/
	int m_HighSpeedRodeNum;

	/*
	* user per road array
	*/
	int* m_pupr;

	/*
	* ��������
	*/
	double m_Speed;//km/h

	/*
	* ͳ��ÿ��RSU�µĳ�����Ŀ
	* ����±����ڼ���λ�ø���(��0��ʼ)
	* �ڲ��±����RSU���
	* ����������ͳ�Ƴ����ֲ����
	*/
	std::vector<std::vector<int>> m_VeUENumPerRSU;

	/*------------------����------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	GTT_HighSpeed() = delete;

	/*
	* ���캯��
	* ����ָ�붼���������ͣ���Ϊ��Ҫ��ʼ��ϵͳ�ĸ���ʵ������
	* �ù��캯��Ҳ�����˸�ģ�����ͼ
	*/
	GTT_HighSpeed(System* t_Context);

	/*
	* ģ���������
	*/
	void configure()override;

	/*
	* ������λ�ø���ʱ���������ĵ��������Ϣ
	*/
	void cleanWhenLocationUpdate()override;

	/*
	* ��ʼ������ʵ������
	*/
	void initialize()override;

	/*
	* �ŵ�����
	*/
	void channelGeneration()override;

	/*
	* �ŵ�ˢ��
	*/
	void freshLoc() override;

	/*
	* д�����λ�ø�����־
	*/
	void writeVeUELocationUpdateLogInfo(std::ofstream &t_File1, std::ofstream &t_File2) override;

	/*
	* ������ž���
	* ����Ĳ�������
	*		����±�Ϊ�������
	*		�ڲ��±�ΪPattern���
	*		���ڲ�listΪ�ó����ڸ�Pattern�µĸ����б�
	* Ŀǰ���дؼ���ţ���ΪRSU�����̫С���������Ժ���
	*/
	void calculateInterference(const std::vector<std::vector<std::list<int>>>& t_RRMInterferenceVec) override;
};