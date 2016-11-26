#pragma once
#include"Enumeration.h"

//ϵͳ�����ķ������
struct SystemConfig {
	/*
	* ����ƽ̨��Linux��Windows�����漰���ļ�·���ĸ�ʽ
	*/
	Platform platform;

	/*
	* GTTģ��ʵ�ֵľ������
	*/
	GTTMode _GTTMode;

	/*
	* RRMģ��ʵ�ֵľ������
	*/
	RRMMode _RRMMode;

	/*
	* WTģ��ʵ�ֵľ������
	*/
	WTMode _WTMode;

	/*
	* �����ܹ���TTI
	*/
	int NTTI;

	/*
	* �߳�����
	*/
	int threadNum;

	/*
	* ����ˢ��λ�õ�����
	*/
	int locationUpdateNTTI;

	/*
	* �������¼������ڣ���λTTI��
	*/
	int periodicEventNTTI;

	/*
	* ����ҵ���¼����ɹ���Lamda
	*/
	double dataLambda;

	/*
	* �����¼����ɹ���Lamda,��λ��/TTI
	*/
	double emergencyLambda;

	/*
	* ��������
	*/
	int VeUENum;

	/*
	* RSU����
	*/
	int RSUNum;

	/*
	* ��վ����
	*/
	int eNBNum;

	/*
	* RoadNum
	*/
	int RoadNum;

	/*
	* TTI��־����
	*/
	bool TTILogIsOn = false;

	/*
	* Event��־����
	*/
	bool eventLogIsOn = false;

	/*
	* Schedule��־����
	*/
	bool scheduleLogIsOn = false;

};

//eNB���ò���
struct eNBConfig {
	SystemConfig* systemConfig;
	int roadId;
	int eNBId;
	double X;
	double Y;
	double AbsX;
	double AbsY;
};

//Road���ò���
struct UrbanRoadConfig {
	SystemConfig* systemConfig;
	int roadId;
	void *eNB;
	int eNBNum;
	int eNBOffset;
	void *lane;
	int laneNum;
	int laneOffset;
};


struct HighSpeedRodeConfig {
	SystemConfig* systemConfig;
	int roadId;
};


//VeUE���ò���
struct VeUEConfig {
	int roadId;
	double X;
	double Y;
	double AbsX;
	double AbsY;
	double V;
	int VeUENum;
	double angle;
};


//����λ�����ò���
struct Location {
	bool manhattan;
	LocationType locationType;
	double distance; //��λ:m
	double distance1; //��λ:m
	double distance2; //��λ:m
	double eNBAntH; //��λ:m
	double VeUEAntH; //��λ:m
	double RSUAntH;//��λ��m
	double posCor[5];

};

//�������ò���
struct Antenna {
	double TxAngle; // degree
	double RxAngle; // degree
	double maxAttenu; // dB
	int byTxAntNum;
	int byRxAntNum;
	double *TxSlantAngle; // degree
	double *RxSlantAngle; // degree
	double *TxAntSpacing;
	double *RxAntSpacing;
	double antGain;
};
