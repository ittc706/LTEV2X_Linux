#pragma once
#include"GTT.h"

// <GTT_Urban>: Geographical Topology and Transport Urban

class GTT_Urban_VeUE :public GTT_VeUE {
	/*------------------��------------------*/
public:
	/*
	* <?>
	*/
	int m_LocationId;
	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	GTT_Urban_VeUE() = delete;
	GTT_Urban_VeUE(VeUEConfig &t_VeUEConfig);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_VeUE  *const getUrbanPoint()override { return this; }
	GTT_HighSpeed_VeUE  *const getHighSpeedPoint()override { throw Exp("RuntimeException"); }
};


class GTT_Urban_RSU :public GTT_RSU {
public:
	/*
	* ���캯��
	*/
	GTT_Urban_RSU();

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_RSU  *const getUrbanPoint()override { return this; }
	GTT_HighSpeed_RSU  *const getHighSpeedPoint()override { throw Exp("RuntimeException"); }
};


class GTT_Urban_eNB :public GTT_eNB {
public:
	/*
	* ��ʼ������
	* ���ù��캯����ԭ���ǹ����ʱ���������û�������
	*/
	void initialize(eNBConfig &t_eNBConfig)override;

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_eNB  *const getUrbanPoint()override { return this; }
	GTT_HighSpeed_eNB  *const getHighSpeedPoint()override { throw Exp("RuntimeException"); }
};


class GTT_Urban_Road :public GTT_Road {
	/*------------------��------------------*/
public:
	/*
	* ��վ��Ŀ
	*/
	int m_eNBNum;

	/*
	* ��վId
	*/
	int m_eNBId;

	/*
	* ����ָ�������ڻ�վ�����ָ��
	*/
	GTT_eNB *m_eNB;

public:
	/*
	* ��ʼ������
	* ���ù��캯����ԭ���ǹ����ʱ���������û�������
	*/
	GTT_Urban_Road(UrbanRoadConfig &t_RoadConfig);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	GTT_Urban_Road  *const getUrbanPoint()override { return this; }
	GTT_HighSpeed_Road  *const getHighSpeedPoint()override { throw Exp("RuntimeException"); }
};


class GTT_Urban :public GTT {
	/*------------------��̬------------------*/
public:
	/*
	* ��������棬���๲��
	*/
	static std::default_random_engine s_Engine;
	/*------------------��------------------*/
private:
	/*
	* ueTopo x�� ����
	*/
	int m_xNum;

	/*
	* ueTopo y�� ����
	*/
	int m_yNum;

	/*
	* ��
	*/
	int m_ueTopoNum;

	/*
	* һ��Road��ue�������
	*/
	double *m_pueTopo;

	/*
	* user per road array
	*/
	int* m_pupr;

	/*
	* ��������
	*/
	double m_Speed;

public:
	/*
	* ͳ��ÿ��RSU�µĳ�����Ŀ
	* ����±�����ڼ���λ�ø���(��0��ʼ)���ڲ��±����RSU���
	*/
	std::vector<std::vector<int>> m_VeUENumPerRSU;

	/*------------------����------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	GTT_Urban() = delete;

	/*
	* ���캯��
	* ����ָ�붼���������ͣ���Ϊ��Ҫ��ʼ��ϵͳ�ĸ���ʵ������
	* �ù��캯��Ҳ�����˸�ģ�����ͼ
	*/
	GTT_Urban(System* t_Context);

	/*
	* ��������
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