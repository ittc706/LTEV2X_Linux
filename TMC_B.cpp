/*
* =====================================================================================
*
*       Filename:  TMC_B.cpp
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
#include<iostream>
#include<random>
#include<sstream>
#include"Exception.h"
#include"TMC_B.h"
#include"System.h"

using namespace std;

TMC_B::TMC_B(System* t_Context):
	TMC(t_Context) {

	//�¼�����������ʼ��
	getContext()->m_EventTTIList = vector<list<int>>(getContext()->m_Config.NTTI);

	//������������ʼ��
	getContext()->m_TTIRSUThroughput = vector<vector<int>>(getContext()->m_Config.NTTI, vector<int>(getContext()->m_Config.RSUNum));
}


void TMC_B::initialize() {
	//��ʼ��VeUE�ĸ�ģ���������
	m_VeUEAry = new TMC_VeUE*[getContext()->m_Config.VeUENum];
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		m_VeUEAry[VeUEId] = new TMC_VeUE();
	}

	//��ʼ��RSU�ĸ�ģ���������
	m_RSUAry = new TMC_RSU*[getContext()->m_Config.RSUNum];
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		m_RSUAry[RSUId] = new TMC_RSU();
	}
}


void TMC_B::buildEventList(ofstream& t_File) {
	/*��ʱ��˳���¼���Id��ʱ����أ�IdԽС���¼�������ʱ��ԽС�����¼�����*/

	default_random_engine dre;//���������
	dre.seed((unsigned)time(NULL));//iomanip
	uniform_real_distribution<double> urd(0, 1);


	//�������ɸ����������������¼�����ʼʱ��(���ʱ�̣���[0 , m_Config.periodicEventNTTI)
	vector<list<int>> startTTIVec(getContext()->m_Config.periodicEventNTTI, list<int>());
	uniform_int_distribution<int> uid(0, getContext()->m_Config.periodicEventNTTI - 1);
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		int startTTI = uid(dre);
		startTTIVec[startTTI].push_back(VeUEId);
	}

	//���ɽ����¼��ķ���ʱ�̣�ÿ��ʱ��۴�Ÿ�ʱ�̷��������¼��ĳ���
	m_VeUEEmergencyNum = vector<int>(getContext()->m_Config.VeUENum, 0);//��ʼ��ͳ����
	int countEmergency = 0;
	vector<list<int>> emergencyEventTriggerTTI(getContext()->m_Config.NTTI);
	if (getContext()->m_Config.emergencyLambda != 0) {
		for (int VeUEId = 0; VeUEId <getContext()->m_Config.VeUENum; VeUEId++) {
			//��������ÿ�������Ľ����¼�����ʱ��
			double T = 0;
			while (T <getContext()->m_Config.NTTI) {
				double u = urd(dre);
				if (u == 0) throw LTEV2X_Exception("uniform_real_distribution���ɷ�Χ�����߽�");
				T = T - (1 / getContext()->m_Config.emergencyLambda)*log(u);
				int IntegerT = static_cast<int>(T);
				if (IntegerT < getContext()->m_Config.NTTI) {
					emergencyEventTriggerTTI[IntegerT].push_back(VeUEId);
					++m_VeUEEmergencyNum[VeUEId];
					++countEmergency;
				}
			}
		}
	}
	cout << "countEmergency: " << countEmergency << endl;


	//��������ҵ���¼��ķ���ʱ�̣�ÿ��ʱ��۴�Ÿ�ʱ�̷�������ҵ���¼��ĳ���
	m_VeUEDataNum = vector<int>(getContext()->m_Config.VeUENum, 0);//��ʼ��ͳ����
	int countData = 0;
	vector<list<int>> dataEventTriggerTTI(getContext()->m_Config.NTTI);
	if (getContext()->m_Config.dataLambda != 0) {
		for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
			//��������ÿ�������Ľ����¼�����ʱ��
			double T = 0;
			while (T < getContext()->m_Config.NTTI) {
				double u = urd(dre);
				if (u == 0) throw LTEV2X_Exception("uniform_real_distribution���ɷ�Χ�����߽�");
				T = T - (1 / getContext()->m_Config.dataLambda)*log(u);
				int IntegerT = static_cast<int>(T);
				if (IntegerT < getContext()->m_Config.NTTI) {
					dataEventTriggerTTI[IntegerT].push_back(VeUEId);
					++m_VeUEDataNum[VeUEId];
					++countData;
				}
			}
		}
	}
	cout << "countData: " << countData << endl;


	//����startTTIVec�������PERIOD�¼��������в�����Ӳ��ɷֲ��Ľ����¼�

	int startTTIOfEachPeriod = 0;//ÿ�����ڵ���ʼʱ��
	while (startTTIOfEachPeriod < getContext()->m_Config.NTTI) {
		//TTIOffsetΪ�����startTTIOfEachPeriod��ƫ����
		for (int TTIOffset = 0; TTIOffset <getContext()->m_Config.periodicEventNTTI; TTIOffset++) {
			//ѹ������¼�
			if (startTTIOfEachPeriod + TTIOffset < getContext()->m_Config.NTTI) {
				list<int> &emergencyList = emergencyEventTriggerTTI[startTTIOfEachPeriod + TTIOffset];
				for (int VeUEId : emergencyList) {
					/*-----------------------ATTENTION-----------------------
					* ����������sEvent�Ķ���Ȼ����ѹ��m_EventVec
					* ����Vector<T>.push_back��ѹ�봫�����ĸ���Ʒ����˻����sEvent�Ŀ������캯��
					* sEventĬ�ϵĿ������캯���ḳֵid��Ա������ǰ�ȫ�ģ�
					*sEvent����Զ��忽�����캯���������ڹ��캯���ĳ�ʼ�����ֿ���id��Ա
					*-----------------------ATTENTION-----------------------*/
					Event evt = Event(VeUEId, startTTIOfEachPeriod + TTIOffset, EMERGENCY);
					getContext()->m_EventVec.push_back(evt);
					getContext()->m_EventTTIList[startTTIOfEachPeriod + TTIOffset].push_back(evt.getEventId());
					--countEmergency;
				}
			}


			//ѹ������ҵ���¼�
			if (startTTIOfEachPeriod + TTIOffset < getContext()->m_Config.NTTI) {
				list<int> &dataList = dataEventTriggerTTI[startTTIOfEachPeriod + TTIOffset];
				for (int VeUEId : dataList) {
					/*-----------------------ATTENTION-----------------------
					* ����������sEvent�Ķ���Ȼ����ѹ��m_EventVec
					* ����Vector<T>.push_back��ѹ�봫�����ĸ���Ʒ����˻����sEvent�Ŀ������캯��
					* sEventĬ�ϵĿ������캯���ḳֵid��Ա������ǰ�ȫ�ģ�
					*sEvent����Զ��忽�����캯���������ڹ��캯���ĳ�ʼ�����ֿ���id��Ա
					*-----------------------ATTENTION-----------------------*/
					Event evt = Event(VeUEId, startTTIOfEachPeriod + TTIOffset, DATA);
					getContext()->m_EventVec.push_back(evt);
					getContext()->m_EventTTIList[startTTIOfEachPeriod + TTIOffset].push_back(evt.getEventId());
					--countData;
				}
			}


			//�����������¼�
			if (startTTIOfEachPeriod + TTIOffset < getContext()->m_Config.NTTI) {
				list<int> &periodList = startTTIVec[TTIOffset];
				for (int VeUEId : periodList) {
					Event evt = Event(VeUEId, startTTIOfEachPeriod + TTIOffset, PERIOD);
					getContext()->m_EventVec.push_back(evt);
					getContext()->m_EventTTIList[startTTIOfEachPeriod + TTIOffset].push_back(evt.getEventId());
				}
			}
		}
		startTTIOfEachPeriod += getContext()->m_Config.periodicEventNTTI;
	}

	//��ӡ�¼�����
	writeEventListInfo(t_File);
}

void TMC_B::processStatistics(
	ofstream& t_FileStatisticsDescription,
	ofstream& t_FileEmergencyDelay, ofstream& t_FilePeriodDelay, ofstream& t_FileDataDelay,
	ofstream& t_FileEmergencyPossion, ofstream& t_FileDataPossion,
	ofstream& t_FileEmergencyConflict, ofstream& t_FilePeriodConflict, ofstream& t_FileDataConflict,
	ofstream& t_FilePackageLoss, ofstream& t_FilePackageTransimit,
	ofstream& t_FileEventLog) {

	stringstream ssPeriod;
	stringstream ssEmergency;
	stringstream ssData;

	/*-----------------------ATTENTION-----------------------
	* endl���ݷ�����Ҫ���������е�����ˢ�µ�����
	* ��������������ʱ������д�����ݣ��ļ����ǿյ�
	*-----------------------ATTENTION-----------------------*/

	/*------------------ͳ�Ƴɹ�������¼���Ŀ------------------*/
	m_TransimitSucceedEventNumPerEventType = vector<int>(3);
	for (Event &event : getContext()->m_EventVec) {
		if (event.isFinished()) {
			switch (event.getMessageType()) {
			case EMERGENCY:
				m_TransimitSucceedEventNumPerEventType[0]++;
				break;
			case PERIOD:
				m_TransimitSucceedEventNumPerEventType[1]++;
				break;
			case DATA:
				m_TransimitSucceedEventNumPerEventType[2]++;
				break;
			}
		}
	}


	t_FileStatisticsDescription << "<EmergencyEventNum>" << m_TransimitSucceedEventNumPerEventType[EMERGENCY] << "</EmergencyEventNum>" << endl;
	t_FileStatisticsDescription << "<PeriodEventNum>" << m_TransimitSucceedEventNumPerEventType[PERIOD] << "</PeriodEventNum>" << endl;
	t_FileStatisticsDescription << "<DataEventNum>" << m_TransimitSucceedEventNumPerEventType[DATA] << "</DataEventNum>" << endl;

	/*------------------ͳ�Ƶȴ�ʱ��------------------*/
	for (Event &event : getContext()->m_EventVec)
		if (event.isFinished()) {
			switch (event.getMessageType()) {
			case EMERGENCY:
				ssEmergency << event.getQueueDelay() << " ";
				break;
			case PERIOD:
				ssPeriod << event.getQueueDelay() << " ";
				break;
			case DATA:
				ssData << event.getQueueDelay() << " ";
				break;
			default:
				throw LTEV2X_Exception("�Ƿ���Ϣ����");
			}
		}
	t_FileEmergencyDelay << ssEmergency.str() << endl;
	t_FilePeriodDelay << ssPeriod.str() << endl;
	t_FileDataDelay << ssData.str() << endl;


	/*------------------ͳ�ƴ���ʱ��------------------*/
	ssPeriod.str("");
	ssEmergency.str("");
	ssData.str("");
	for (Event &event : getContext()->m_EventVec)
		if (event.isFinished()) {
			switch (event.getMessageType()) {
			case EMERGENCY:
				ssEmergency << event.getSendDelay() << " ";
				break;
			case PERIOD:
				ssPeriod << event.getSendDelay() << " ";
				break;
			case DATA:
				ssData << event.getSendDelay() << " ";
				break;
			default:
				throw LTEV2X_Exception("�Ƿ���Ϣ����");
			}
		}
	t_FileEmergencyDelay << ssEmergency.str() << endl;
	t_FilePeriodDelay << ssPeriod.str() << endl;
	t_FileDataDelay << ssData.str() << endl;


	/*------------------ͳ�ƽ����¼��ֲ����------------------*/
	for (int num : m_VeUEEmergencyNum)
		t_FileEmergencyPossion << num << " ";
	t_FileEmergencyPossion << endl;//����ܹؼ�����������������ˢ�µ�����

								   //ͳ������ҵ���¼��ֲ����
	for (int num : m_VeUEDataNum)
		t_FileDataPossion << num << " ";
	t_FileDataPossion << endl;//����ܹؼ�����������������ˢ�µ�����

							  //ͳ�Ƴ�ͻ���
	ssPeriod.str("");
	ssEmergency.str("");
	ssData.str("");
	for (Event &event : getContext()->m_EventVec) {
		switch (event.getMessageType()) {
		case EMERGENCY:
			ssEmergency << event.getConflictNum() << " ";
			break;
		case PERIOD:
			ssPeriod << event.getConflictNum() << " ";
			break;
		case DATA:
			ssData << event.getConflictNum() << " ";
			break;
		default:
			throw LTEV2X_Exception("�Ƿ���Ϣ����");
		}
	}
	t_FileEmergencyConflict << ssEmergency.str() << endl;
	t_FilePeriodConflict << ssPeriod.str() << endl;
	t_FileDataConflict << ssData.str() << endl;
	writeEventLogInfo(t_FileEventLog);

	/*------------------ͳ��������------------------*/
	vector<int> tmpTTIThroughput(getContext()->m_Config.NTTI);
	vector<int> tmpRSUThroughput(getContext()->m_Config.RSUNum);
	for (int tmpTTI = 0; tmpTTI < getContext()->m_Config.NTTI; tmpTTI++) {
		for (int tmpRSUId = 0; tmpRSUId < getContext()->m_Config.RSUNum; tmpRSUId++) {
			tmpTTIThroughput[tmpTTI] += getContext()->m_TTIRSUThroughput[tmpTTI][tmpRSUId];
			tmpRSUThroughput[tmpRSUId] += getContext()->m_TTIRSUThroughput[tmpTTI][tmpRSUId];
		}
	}

	//��TTIΪ��λͳ��������
	for (int throughput : tmpTTIThroughput)
		g_FileTTIThroughput << throughput << " ";
	g_FileTTIThroughput << endl;

	//��RSUΪ��λͳ��������
	for (int throughput : tmpRSUThroughput)
		g_FileRSUThroughput << throughput << " ";
	g_FileRSUThroughput << endl;


	/*------------------ͳ�ƶ�����------------------*/
	int transimitPackageNum = 0;
	int lossPacketNum = 0;
	for (Event &event : getContext()->m_EventVec) {
		transimitPackageNum += event.getTransimitPackageNum();
		lossPacketNum += event.getPacketLossNum();
		for (double &d : event.getPackageLossDistanceVec())
			t_FilePackageLoss << d << endl;
		for (double &d : event.getPackageTransimitDistanceVec())
			g_FilePackageTransimit << d << endl;
	}
	t_FileStatisticsDescription << "<TransimitPackageNum>" << transimitPackageNum <<"</TransimitPackageNum>"<< endl;
	t_FileStatisticsDescription << "<PackageLossRate>" << (double)lossPacketNum / (double)transimitPackageNum << "</PackageLossRate>" << endl;
}

void TMC_B::writeEventListInfo(ofstream &t_File) {
	for (int i = 0; i < getContext()->m_Config.NTTI; i++) {
		t_File << "[ TTI = " << left << setw(3) << i << " ]" << endl;
		t_File << "{" << endl;
		for (int eventId : getContext()->m_EventTTIList[i]) {
			Event& e = getContext()->m_EventVec[eventId];
			t_File << "    " << e.toString() << endl;
		}
		t_File << "}\n\n" << endl;
	}
}

void TMC_B::writeEventLogInfo(ofstream &t_File) {
	for (int eventId = 0; eventId < static_cast<int>(getContext()->m_EventVec.size()); eventId++) {
		string s;
		switch (getContext()->m_EventVec[eventId].getMessageType()) {
		case EMERGENCY:
			s = "EMERGENCY";
			break;
		case PERIOD:
			s = "PERIOD";
			break;
		case DATA:
			s = "DATA";
			break;
		}
		t_File << "EventId = " << eventId << endl;
		t_File << "{" << endl;
		t_File << "    " << "VeUEId = " << getContext()->m_EventVec[eventId].getVeUEId() << endl;
		t_File << "    " << "MessageType = " << s << endl;
		t_File << "    " << "SendDelay = " << getContext()->m_EventVec[eventId].getSendDelay() << "(TTI)" << endl;
		t_File << "    " << "QueueDelay = " << getContext()->m_EventVec[eventId].getQueueDelay() << "(TTI)" << endl;
		t_File << "    " << "ProcessDelay = " << getContext()->m_EventVec[eventId].getProcessDelay() << "(TTI)" << endl;
		t_File << getContext()->m_EventVec[eventId].toLogString(1);
		t_File << "}" << endl;
	}
}


