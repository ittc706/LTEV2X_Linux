#pragma once
#include<fstream>
#include<random>
#include<memory>
#include"WT.h"
#include"Matrix.h"


//<WT_B>: Wireless Transmission Base

class WT_B :public WT {
	/*------------------��̬------------------*/
public:
	static std::default_random_engine s_Engine;

	/*------------------��------------------*/
public:
	/*
	* ����������
	*/
	int m_Nt;

	/*
	* ����������
	*/
	int m_Nr;

	/*
	* ���书��
	*/
	double m_Pt;

	/*
	* ����·�����
	*/
	double m_Ploss;

	/*
	* ���ž���·�����
	*/
	std::vector<double> m_PlossInterference;

	/*
	* ��˹�����Ĺ��ʣ���λ��mw
	*/
	double m_Sigma;

	/*
	* ÿ�����ز���һ��Nr*Nt���ŵ�����
	*/
	Matrix m_H;

	/*
	* ÿ�����ز���һ��Nr*Nt�ĸ����ŵ�����
	* �±�Ϊ����Դ���
	*/
	std::vector<Matrix> m_HInterference;

	/*
	* ���³�Ա��Ϊָ�룬��new�����ڴ棬��Ϊ�����ͬWT_Bʵ���Ĺ�����Դ��ֻ����initialize()��ʼ��һ��
	*/
	std::shared_ptr<std::vector<double>> m_QPSK_MI;

	/*------------------����------------------*/
public:
	/*
	* ���캯��
	* �ù��캯�������˸�ģ�����ͼ
	* ����ָ���Ա����ϵͳ���еĶ�Ӧ��Աָ�룬����ͬһʵ��
	*/
	WT_B(System* t_Context);

private:
	/*
	* �������캯��
	* ����Ϊ˽�б�������
	*/
	WT_B(const WT_B& t_WT_B);

public:
	/*
	* ��ʼ��RSU VeUE�ڸõ�Ԫ���ڲ���
	*/
	void initialize() override;

	/*
	* ��ȡ��ģ���һ������
	*/
	WT* getCopy()override;

	/*
	* �ͷŸ�ģ��Ŀ���
	*/
	void freeCopy()override;

	/*
	* �����ظɱ�
	*/
	double SINRCalculate(int t_VeUEId, int t_SubCarrierIdxStart, int t_SubCarrierIdxEnd, int t_PatternIdx) override;

	/*
	* �����ظɱȣ�MRC
	*/
	double SINRCalculateMRC(int t_VeUEId, int t_SubCarrierIdxStart, int t_SubCarrierIdxEnd, int t_PatternIdx);

	/*
	* �����ظɱȣ�MMSE
	*/
	double SINRCalculateMMSE(int t_VeUEId, int t_SubCarrierIdxStart, int t_SubCarrierIdxEnd, int t_PatternIdx);

private:
	/*
	* ÿ�ε���SINRCalculateǰ��Ҫ���в�������
	*/
	void configuration(int t_VeUEId, int t_PatternIdx, int t_SubCarrierNum);

	/*
	* ��ȡ��Ӧ���ز����ŵ���Ӧ����
	*/
	Matrix readH(int t_VeUEId, int t_SubCarrierIdx);

	/*
	* ��ȡ��Ӧ�����ڶ�Ӧ���ز��ϵĸ��ž�������
	*/
	std::vector<Matrix> readInterferenceH(int t_VeUEId, int t_SubCarrierIdx, int t_PatternIdx);

	/*
	* ���ַ������㷨
	*/
	int closest(std::vector<double> t_Vec, double t_Target);

	/*
	* ���
	*/
	double getMutualInformation(std::vector<double> t_Vec, int t_Index);
};