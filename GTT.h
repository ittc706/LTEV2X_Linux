#pragma once
#include<random>
#include<vector>
#include<list>
#include<tuple>
#include"Config.h"

// <GTT>: Geographical Topology and Transport

class VeUE;
class GTT_Urban_VeUE;
class GTT_HighSpeed_VeUE;
class IMTA;

class GTT_VeUE {
	/*------------------��̬------------------*/
public:
	/*
	* ��������
	*/
	static int s_VeUECount;
	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪVeUE���ݽ�����ϵͳ��VeUE����
	*/
	VeUE* m_This=nullptr;

public:
	/*
	* ����ID
	*/
	const int m_VeUEId = s_VeUECount++;

	/*
	* ���ڵ�·��RoadId
	*/
	int m_RoadId;

	/*
	* ��Ժ����꣬������
	*/
	double m_X;
	double m_Y;

	/*
	* ���Ժ����꣬������
	*/
	double m_AbsX;
	double m_AbsY;

	/*
	* �����ٶ�
	*/
	double m_V;

	/*
	* <?>
	*/
	double m_VAngle;

	/*
	* <?>
	*/
	double m_FantennaAngle;

	/*
	* <?>
	*/
	IMTA *m_IMTA = nullptr;

	/*
	* �������ڵ�RSUId
	*/
	int m_RSUId;

	/*
	* �������ڴر��
	*/
	int m_ClusterIdx;

	/*
	* ����������Ŀ
	*/
	int m_Nt;

	/*
	* ����������Ŀ
	*/
	int m_Nr;

	/*
	* ·�����
	*/
	double m_Ploss;

	/*
	* �ŵ���Ӧ����
	*/
	double* m_H = nullptr;

	/*
	* ����������RSU֮��ľ���
	*/
	double* m_Distance = nullptr;

	/*
	* �����������Ե�ǰ�����ĸ���·����ģ�WT_Bģ����Ҫ
	* �±꣺VeUEId(����һ��ʼ�Ϳ��ٺ����г����Ĳ�λ���ò��size����)
	*/
	std::vector<double> m_InterferencePloss;

	/*
	* �����������Ե�ǰ�������ŵ���Ӧ����WT_Bģ����Ҫ
	* �±꣺���ų�����VeUEId��VeUEId(����һ��ʼ�Ϳ��ٺ����г����Ĳ�λ���ò��size����)
	*/
	std::vector<double*> m_InterferenceH;

	/*
	* ����λ�ø�����־��Ϣ
	*/
	std::list<std::tuple<int, int>> m_LocationUpdateLogInfoList;

	/*------------------����------------------*/
public:
	/*
	* ���캯��
	*/
	GTT_VeUE() {}

	/*
	* �����������ͷ�ָ��
	*/
	~GTT_VeUE();

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	virtual GTT_Urban_VeUE  *const getUrbanPoint() = 0;
	virtual GTT_HighSpeed_VeUE  *const getHighSpeedPoint() = 0;

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
class GTT_Urban_RSU;
class GTT_HighSpeed_RSU;

class GTT_RSU {
	/*------------------��̬------------------*/
public:
	/*
	* ��������
	*/
	static int s_RSUCount;
	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪRSU���ݽ�����ϵͳ��RSU����
	*/
	RSU* m_This = nullptr;

public:
	/*
	* RSUId
	*/
	int m_RSUId = s_RSUCount++;

	/*
	* ���Ժ����꣬������
	*/
	double m_AbsX;
	double m_AbsY;

	/*
	* <?>
	*/
	IMTA *m_IMTA = nullptr;

	/*
	* <?>
	*/
	double m_FantennaAngle;

	/*
	* ��ǰRSU��Χ�ڵ�VeUEId�������,RRMģ����Ҫ
	*/
	std::list<int> m_VeUEIdList;

	/*
	* һ��RSU���Ƿ�Χ�ڵĴصĸ���,RRMģ����Ҫ
	*/
	int m_ClusterNum;

	/*
	* ���ÿ���ص�VeUE��Id������,�±����صı��
	*/
	std::vector<std::list<int>> m_ClusterVeUEIdList;

	/*------------------����------------------*/
	/*
	* �����������ͷ�ָ��
	*/
	~GTT_RSU();

	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString(int t_NumTab);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	virtual GTT_Urban_RSU  *const getUrbanPoint() = 0;
	virtual GTT_HighSpeed_RSU  *const getHighSpeedPoint() = 0;

	/*
	* ȡ��ϵͳ��System��RSU��ָ��
	*/
	RSU* getSystemPoint() { return m_This; }

	/*
	* ����ϵͳ��System��RSU��ָ��
	*/
	void setSystemPoint(RSU* t_Point) { m_This = t_Point; }
};

class eNB;
class GTT_Urban_eNB;
class GTT_HighSpeed_eNB;

class GTT_eNB {
	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪeNB���ݽ�����ϵͳ��eNB����
	*/
	eNB* m_This = nullptr;
public:
	/*
	* ���ڵ�·Id
	*/
	int m_RoadId;

	/*
	* ��վId
	*/
	int m_eNBId;

	/*
	* ��վ��Ժ�������
	*/
	double m_X, m_Y;

	/*
	* ��վ���Ժ�������
	*/
	double m_AbsX, m_AbsY;

	/*
	* �û�վ�е�RSU����(�洢RSU��Id)
	*/
	std::list<int> m_RSUIdList;

	/*
	* �û�վ�е�VeUE����(�洢VeUE��Id)
	*/
	std::list<int> m_VeUEIdList;


	/*------------------����------------------*/
public:

	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString(int t_NumTab);

	/*
	* ��ʼ������
	* ���ù��캯����ԭ���ǹ����ʱ���������û�������
	*/
	virtual void initialize(eNBConfig &t_eNBConfig) = 0;

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	virtual GTT_Urban_eNB  *const getUrbanPoint() = 0;
	virtual GTT_HighSpeed_eNB  *const getHighSpeedPoint() = 0;

	/*
	* ȡ��ϵͳ��System��eNB��ָ��
	*/
	eNB* getSystemPoint() { return m_This; }

	/*
	* ����ϵͳ��System��eNB��ָ��
	*/
	void setSystemPoint(eNB* t_Point) { m_This = t_Point; }
};

class Road;
class GTT_Urban_Road;
class GTT_HighSpeed_Road;

class GTT_Road {
	/*------------------��------------------*/
private:
	/*
	* ָ�����ڲ�ͬ��ԪRoad���ݽ�����ϵͳ��Road����
	*/
	Road* m_This = nullptr;
public:
	/*
	* ��·Id
	*/
	int m_RoadId;

	/*
	* ���Ժ�������������
	*/
	double m_AbsX;
	double m_AbsY;

	/*
	* <?>
	*/
	int  m_upr;

	/*------------------����------------------*/
public:
	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString(int t_NumTab);

	/*
	* ����ȡ��ָ��ʵ�����͵�ָ��
	*/
	virtual GTT_Urban_Road  *const getUrbanPoint() = 0;
	virtual GTT_HighSpeed_Road  *const getHighSpeedPoint() = 0;

	/*
	* ȡ��ϵͳ��System��Road��ָ��
	*/
	Road* getSystemPoint() { return m_This; }

	/*
	* ����ϵͳ��System��Road��ָ��
	*/
	void setSystemPoint(Road* t_Point) { m_This = t_Point; }
};


class System;
class GTT {
	/*------------------��------------------*/
private:
	/*
	* ָ��ϵͳ��ָ��
	*/
	System* m_Context;
public:
	/*
	* GTT��Ԫ��ͼ�µĻ�վ����
	*/
	GTT_eNB** m_eNBAry;

	/*
	* GTT��Ԫ��ͼ�µĵ�·����
	*/
	GTT_Road** m_RoadAry;

	/*
	* GTT��Ԫ��ͼ�µ�RSU����
	*/
	GTT_RSU** m_RSUAry;

	/*
	* VeUE����
	* ��һά�ȵ�ָ��ָ�����飬��������ָ��GTT_VeUEʵ���ָ��
	* Ϊʲô��������ָ�룬��Ϊ��Ҫʵ�ֶ�̬
	*/
	GTT_VeUE** m_VeUEAry;

	/*------------------�ӿ�------------------*/
public:
	/*
	* Ĭ�Ϲ��캯������Ϊɾ��
	*/
	GTT() = delete;

	/*
	* ���캯��
	*/
	GTT(System* t_Context) : m_Context(t_Context) {}

	/*
	* ��������
	*/
	~GTT();

	/*
	* ��ȡϵͳ���ָ��
	*/
	System* getContext() { return m_Context; }

	/*
	* ģ���������
	*/
	virtual void configure() = 0;

	/*
	* ������λ�ø���ʱ���������ĵ��������Ϣ
	*/
	virtual void cleanWhenLocationUpdate() = 0;

	/*
	* ��ʼ������ʵ������
	*/
	virtual void initialize() = 0;

	/*
	* �ŵ�����
	*/
	virtual void channelGeneration() = 0;

	/*
	* �ŵ�ˢ��
	*/
	virtual void freshLoc() = 0;

	/*
	* д�����λ�ø�����־
	*/
	virtual void writeVeUELocationUpdateLogInfo(std::ofstream &t_File1, std::ofstream &t_File2) = 0;

	/*
	* ������ž���
	* ����Ĳ�������
	*		����±�Ϊ�������
	*		�ڲ��±�ΪPattern���
	*		���ڲ�listΪ�ó����ڸ�Pattern�µĸ����б�
	* Ŀǰ���дؼ���ţ���ΪRSU�����̫С���������Ժ���
	*/
	virtual void calculateInterference(const std::vector<std::vector<std::list<int>>>& t_RRMInterferenceVec) = 0;
};
