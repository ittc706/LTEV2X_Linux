/*
* =====================================================================================
*
*       Filename:  System.cpp
*
*    Description:  TMCģ��
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<iomanip>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include<stdexcept>
#include"System.h"

#include"GTT.h"
#include"GTT_HighSpeed.h"
#include"GTT_Urban.h"

#include"RRM.h"
#include"RRM_ICC_DRA.h"
#include"RRM_TDM_DRA.h"
#include"RRM_RR.h"

#include"TMC.h"
#include"TMC_B.h"

#include"WT.h"
#include"WT_B.h"

#include"VUE.h"
#include"RSU.h"
#include"eNB.h"
#include"Road.h"

#include"Config.h"
#include"Function.h"
#include"ConfigLoader.h"
#include"Log.h"


using namespace std;

void System::process() {
	long double programStart = clock();

	//��������
	configure();

	//�����ʼ��
	initialization();

	//�����¼�����
	m_TMCPoint->buildEventList(g_FileEventListInfo);

	//��ʼ����
	for (int count = 0;count < m_Config.NTTI;count++) {
		cout << "Current TTI = " << m_TTI << endl;
		//����λ�ø���
		if (count % m_Config.locationUpdateNTTI == 0) {
			m_GTTPoint->channelGeneration();
			m_GTTPoint->cleanWhenLocationUpdate();
			m_RRMPoint->cleanWhenLocationUpdate();
		}
		//��ʼ��Դ����
		m_RRMPoint->schedule();
		m_TTI++;
	}

	cout.setf(ios::fixed);
	double timeFactor;
	if (m_Config.platform == Windows)
		timeFactor = 1000L;
	else
		timeFactor = 1000000L;
	cout << "�����ŵ������ʱ��" << m_RRMPoint->m_GTTTimeConsume / timeFactor << " s\n" << endl;
	cout << "SINR�����ʱ��" << m_RRMPoint->m_WTTimeConsume / timeFactor << " s\n" << endl;
	cout.unsetf(ios::fixed);

	//�������ҵ��ʱ������
	m_TMCPoint->processStatistics(
		g_FileStatisticsDescription,
		g_FileEmergencyDelayStatistics, g_FilePeriodDelayStatistics, g_FileDataDelayStatistics,
		g_FileEmergencyPossion, g_FileDataPossion,
		g_FileEmergencyConflictNum, g_FilePeriodConflictNum, g_FileDataConflictNum,
		g_FilePackageLoss, g_FilePackageTransimit,
		g_FileEventLogInfo);

	//��ӡ��������λ�ø�����־��Ϣ
	m_GTTPoint->writeVeUELocationUpdateLogInfo(g_FileVeUELocationUpdateLogInfo, g_FileVeUENumPerRSULogInfo);

	//���������ʱ
	long double programEnd = clock();
	cout.setf(ios::fixed);
	cout << "\nRunning Time :" << setprecision(1) << (programEnd - programStart) / timeFactor << " s\n" << endl;
	cout.unsetf(ios::fixed);
}

void System::configure() {//ϵͳ�����������
	srand((unsigned)time(NULL));//���������������������

	ConfigLoader configLoader;

	//�������жϵ�ǰ��ƽ̨������·���ı�ʾ������ƽ̨�µĲ������ж�
	ifstream inPlatformWindows("Config\\systemConfig.xml"),
		inPlatformLinux("Config/systemConfig.xml");
	
	if (inPlatformWindows.is_open()) {
		m_Config.platform = Windows;
		cout << "����ǰ��ƽ̨Ϊ��Windows" << endl;	
	}
	else if (inPlatformLinux.is_open()) {
		m_Config.platform = Linux;
		cout << "����ǰ��ƽ̨Ϊ��Linux" << endl;	
	}
	else
		throw logic_error("PlatformError");

	//��ʼ�����������
	logFileConfig(m_Config.platform);

	/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>��ʼ����ϵͳ�����ļ�<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
	switch (m_Config.platform) {
	case Windows:
		configLoader.resolvConfigPath("Config\\systemConfig.xml");
		break;
	case Linux:
		configLoader.resolvConfigPath("Config/systemConfig.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	stringstream ss;

	const string nullString("");
	string temp;

	if ((temp = configLoader.getParam("NTTI")) != nullString) {
		ss << temp;
		ss >> m_Config.NTTI;
		ss.clear();//�����־λ
		ss.str("");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("periodicEventNTTI")) != nullString) {
		ss << temp;
		ss >> m_Config.periodicEventNTTI;
		ss.clear();//�����־λ
		ss.str("");
	}
	else
		throw logic_error("ConfigLoaderError");


	if ((temp = configLoader.getParam("emergencyLambda")) != nullString) {
		ss << temp;
		ss >> m_Config.emergencyLambda;
		ss.clear();//�����־λ
		ss.str("");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("dataLambda")) != nullString) {
		ss << temp;
		ss >> m_Config.dataLambda;
		ss.clear();//�����־λ
		ss.str("");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("locationUpdateNTTI")) != nullString) {
		ss << temp;
		ss >> m_Config.locationUpdateNTTI;
		ss.clear();//�����־λ
		ss.str("");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("GTTMode")) != nullString) {
		if (temp == "URBAN") {
			m_Config._GTTMode = URBAN;
			cout << "GTT��Ԫ��URBANģʽ" << endl;
		}
		else if (temp == "HIGHSPEED") {
			m_Config._GTTMode = HIGHSPEED;
			cout << "GTT��Ԫ��HIGHSPEEDģʽ" << endl;
		}
		else
			throw logic_error("�������˵�Ԫ�������ô���");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("RRMMode")) != nullString) {
		if (temp == "TDM_DRA") {
			m_Config._RRMMode = TDM_DRA;
			cout << "RRM��Ԫ��TDM_DRAģʽ" << endl;
		}
		else if (temp == "ICC_DRA") {
			m_Config._RRMMode = ICC_DRA;
			cout << "RRM��Ԫ��ICC_DRAģʽ" << endl;
		}
		else if (temp == "RR") {
			m_Config._RRMMode = RR;
			cout << "RRM��Ԫ��RRģʽ" << endl;
		}
		else
			throw logic_error("������Դ����Ԫ�������ô���");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("ThreadNum")) != nullString) {
		ss << temp;
		ss >> m_Config.threadNum;
		ss.clear();//�����־λ
		ss.str("");
		cout << "���ٵ��߳�����Ϊ: " << m_Config.threadNum << endl;
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("WTMode")) != nullString) {
		if (temp == "SINR_MRC") {
			m_Config._WTMode = SINR_MRC;
			cout << "WT��Ԫ��SINR_MRCģʽ" << endl;
		}
		else if (temp == "SINR_MMSE") {
			m_Config._WTMode = SINR_MMSE;
			cout << "WT��Ԫ��SINR_MMSEģʽ" << endl;
		}
		else
			throw logic_error("���ߴ��䵥Ԫ�������ô���");
	}
	else
		throw logic_error("ConfigLoaderError");


	/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>��ʼ������־�����ļ�<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
	switch (m_Config.platform) {
	case Windows:
		configLoader.resolvConfigPath("Config\\LogControlConfig.xml");
		break;
	case Linux:
		configLoader.resolvConfigPath("Config/LogControlConfig.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	if ((temp = configLoader.getParam("TTILog")) != nullString) {
		if (temp == "ON")
			m_Config.TTILogIsOn = true;
		else if (temp == "OFF")
			m_Config.TTILogIsOn = false;
		else
			throw logic_error("TTI��־�������ô���");
		ss.clear();//�����־λ
		ss.str("");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("EventLog")) != nullString) {
		if (temp == "ON")
			m_Config.eventLogIsOn = true;
		else if (temp == "OFF")
			m_Config.eventLogIsOn = false;
		else
			throw logic_error("Event��־�������ô���");
		ss.clear();//�����־λ
		ss.str("");
		Event::s_LogIsOn = m_Config.eventLogIsOn;
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = configLoader.getParam("ScheduleLog")) != nullString) {
		if (temp == "ON")
			m_Config.scheduleLogIsOn = true;
		else if (temp == "OFF")
			m_Config.scheduleLogIsOn = false;
		else
			throw logic_error("Schedule��־�������ô���");
		ss.clear();//�����־λ
		ss.str("");
	}
	else
		throw logic_error("ConfigLoaderError");
}


void System::initialization() {
	m_TTI = 0;	

	//GTTģ���ʼ��
	initializeGTTModule();

	//WTģ���ʼ��
	initializeWTModule();

	//RRMģ���ʼ��
	initializeRRMModule();

	//TMCģ���ʼ��
	initializeTMCModule();

	initializeNON();
}


void System::initializeGTTModule() {
	switch (m_Config._GTTMode) {
	case URBAN:
		m_GTTPoint = new GTT_Urban(this);
		break;
	case HIGHSPEED:
		m_GTTPoint = new GTT_HighSpeed(this);
		break;
	}
	//��ʼ���������˲���
	m_GTTPoint->configure();

	//��ʼ��eNB��Rode��RSU��VUE������
	m_GTTPoint->initialize();
}

void System::initializeWTModule() {
	m_WTPoint = new WT_B(this);
	m_WTPoint->initialize();//ģ���ʼ��
}


void System::initializeRRMModule() {
	switch (m_Config._RRMMode) {
	case RR:
		m_RRMPoint = new RRM_RR(this);
		break;
	case TDM_DRA:
		m_RRMPoint = new RRM_TDM_DRA(this);
		break;
	case ICC_DRA:
		m_RRMPoint = new RRM_ICC_DRA(this);
		break;
	default:
		break;
	}
	m_RRMPoint->initialize();//ģ���ʼ��
}


void System::initializeTMCModule() {
	m_TMCPoint = new TMC_B(this);
	m_TMCPoint->initialize();//ģ���ʼ��
}


void System::initializeNON() {
	//ϵͳVeUE�������Ԫ��VeUE��ͼ��������
	m_VeUEAry = new VeUE[m_Config.VeUENum];
	for (int VeUEId = 0; VeUEId < m_Config.VeUENum; VeUEId++) {
		m_VeUEAry[VeUEId].m_GTT = m_GTTPoint->m_VeUEAry[VeUEId];
		m_GTTPoint->m_VeUEAry[VeUEId]->setSystemPoint(&m_VeUEAry[VeUEId]);

		m_VeUEAry[VeUEId].m_RRM = m_RRMPoint->m_VeUEAry[VeUEId];
		m_RRMPoint->m_VeUEAry[VeUEId]->setSystemPoint(&m_VeUEAry[VeUEId]);

		m_VeUEAry[VeUEId].m_WT = m_WTPoint->m_VeUEAry[VeUEId];
		(m_WTPoint->m_VeUEAry[VeUEId])->setSystemPoint(&m_VeUEAry[VeUEId]);

		m_VeUEAry[VeUEId].m_TMC= m_TMCPoint->m_VeUEAry[VeUEId];
		m_TMCPoint->m_VeUEAry[VeUEId]->setSystemPoint(&m_VeUEAry[VeUEId]);
	}

	//ϵͳRSU�������Ԫ��RSU��ͼ��������
	m_RSUAry = new RSU[m_Config.RSUNum];
	for (int RSUId = 0; RSUId < m_Config.RSUNum; RSUId++) {
		m_RSUAry[RSUId].m_GTT = m_GTTPoint->m_RSUAry[RSUId];
		m_GTTPoint->m_RSUAry[RSUId]->setSystemPoint(&m_RSUAry[RSUId]);

		m_RSUAry[RSUId].m_RRM = m_RRMPoint->m_RSUAry[RSUId];
		m_RRMPoint->m_RSUAry[RSUId]->setSystemPoint(&m_RSUAry[RSUId]);

		m_RSUAry[RSUId].m_WT = m_WTPoint->m_RSUAry[RSUId];
		(m_WTPoint->m_RSUAry[RSUId])->setSystemPoint(&m_RSUAry[RSUId]);

		m_RSUAry[RSUId].m_TMC = m_TMCPoint->m_RSUAry[RSUId];
		m_TMCPoint->m_RSUAry[RSUId]->setSystemPoint(&m_RSUAry[RSUId]);

		//����ȵ�������Ԫ��RSU�������Ӻ󣬲��ܶ�RRM��Ԫ�ڵ�RSU������г�ʼ��
		m_RRMPoint->m_RSUAry[RSUId]->initialize();
	}

	//ϵͳeNB�������Ԫ��eNB��ͼ��������
	m_eNBAry = new eNB[m_Config.eNBNum];
	for (int eNBId = 0; eNBId < m_Config.eNBNum; eNBId++) {
		m_eNBAry[eNBId].m_GTT = m_GTTPoint->m_eNBAry[eNBId];
		m_GTTPoint->m_eNBAry[eNBId]->setSystemPoint(&m_eNBAry[eNBId]);
	}

	//ϵͳRoad�������Ԫ��Road��ͼ��������
	m_RoadAry = new Road[m_Config.RoadNum];
	for (int roadId = 0; roadId < m_Config.RoadNum; roadId++) {
		m_RoadAry[roadId].m_GTT = m_GTTPoint->m_RoadAry[roadId];
		m_GTTPoint->m_RoadAry[roadId]->setSystemPoint(&m_RoadAry[roadId]);
	}
}


System::~System() {
	//����ģ��ָ��
	Delete::safeDelete(m_TMCPoint);
	Delete::safeDelete(m_RRMPoint);
	Delete::safeDelete(m_GTTPoint);
	Delete::safeDelete(m_WTPoint);

	//�����ʵ��������
	Delete::safeDelete(m_eNBAry, true);
	Delete::safeDelete(m_RSUAry, true);
	Delete::safeDelete(m_VeUEAry, true);
	Delete::safeDelete(m_RoadAry, true);

    //�ر��ļ���
	g_FileTemp.close();

	g_FileVeUELocationUpdateLogInfo.close();
	g_FileVeUENumPerRSULogInfo.close();
	g_FileLocationInfo.close();

	g_FileScheduleInfo.close();
	g_FileClasterPerformInfo.close();
	g_FileEventListInfo.close();
	g_FileTTILogInfo.close();
	g_FileEventLogInfo.close();

	g_FileEmergencyDelayStatistics.close();
	g_FileEmergencyPossion.close();
	g_FileDataPossion.close();
	g_FileEmergencyConflictNum.close();
	g_FileTTIThroughput.close();
	g_FileRSUThroughput.close();
}

