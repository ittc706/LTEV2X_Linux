/*
* =====================================================================================
*
*       Filename:  GTT_HighSpeed.cpp
*
*    Description:  TMCģ��
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  LK,WYB
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<tuple>
#include<iomanip>
#include<iostream>
#include<string.h>
#include"GTT_HighSpeed.h"
#include"Exception.h"
#include"Function.h"
#include"System.h"

using namespace std;
using namespace ns_GTT_HighSpeed;


GTT_HighSpeed_VeUE::GTT_HighSpeed_VeUE(VeUEConfig &t_VeUEConfig) {
	m_RoadId = t_VeUEConfig.roadId;
	m_X = t_VeUEConfig.X;
	m_Y = t_VeUEConfig.Y;
	m_AbsX = t_VeUEConfig.AbsX;
	m_AbsY = t_VeUEConfig.AbsY;
	m_V = t_VeUEConfig.V / 3.6f;//��km/h����Ϊm/s�����ڳ�����λ�ø���

	if (m_RoadId <= 2)
		m_VAngle = 0;
	else
		m_VAngle = 180;

	randomUniform(&m_FantennaAngle, 1, 180.0f, -180.0f, false);

	m_Nt = 1;
	m_Nr = 2;
	m_H = new double[2 * 1024 * 2];
	m_InterferencePloss = vector<double>(t_VeUEConfig.VeUENum, 0);
	m_InterferenceH = vector<double*>(t_VeUEConfig.VeUENum, nullptr);
}


GTT_HighSpeed_RSU::GTT_HighSpeed_RSU() {
	m_AbsX = ns_GTT_HighSpeed::gc_RSUTopoRatio[m_RSUId * 2 + 0] * 100;
	m_AbsY = ns_GTT_HighSpeed::gc_RSUTopoRatio[m_RSUId * 2 + 1];
	randomUniform(&m_FantennaAngle, 1, 180.0f, -180.0f, false);
	g_FileLocationInfo << toString(0);
	m_ClusterNum = ns_GTT_HighSpeed::gc_RSUClusterNum;
	m_ClusterVeUEIdList = vector<list<int>>(m_ClusterNum);
}


void GTT_HighSpeed_eNB::initialize(eNBConfig &t_eNBConfig) {
	m_eNBId = t_eNBConfig.eNBId;
	m_AbsX = ns_GTT_HighSpeed::gc_eNBTopo[m_eNBId * 2 + 0];
	m_AbsY = ns_GTT_HighSpeed::gc_eNBTopo[m_eNBId * 2 + 1];
	g_FileLocationInfo << toString(0);
}


GTT_HighSpeed_Road::GTT_HighSpeed_Road(HighSpeedRodeConfig &t_RoadHighSpeedConfig) {
	m_RoadId = t_RoadHighSpeedConfig.roadId;
	m_AbsX = 0.0f;
	m_AbsY = ns_GTT_HighSpeed::gc_LaneTopoRatio[m_RoadId * 2 + 1] * ns_GTT_HighSpeed::gc_LaneWidth;
}


default_random_engine GTT_HighSpeed::s_Engine((unsigned)time(NULL));


GTT_HighSpeed::GTT_HighSpeed(System* t_Context) :
	GTT(t_Context) {}


void GTT_HighSpeed::configure() {
	getContext()->m_Config.eNBNum = gc_eNBNumber;
	m_HighSpeedRodeNum = gc_LaneNumber;
	getContext()->m_Config.RSUNum = gc_RSUNumber;//Ŀǰֻ��ʾUE RSU��
	m_pupr = new int[m_HighSpeedRodeNum];
	getContext()->m_Config.VeUENum = 0;
	double Lambda = gc_Length*3.6 / (2.5 * m_Speed);
	srand((unsigned)time(NULL));
	for (int temp = 0; temp != m_HighSpeedRodeNum; ++temp)
	{
		int k = 0;
		long double p = 1.0;
		long double l = exp(-Lambda);  /* Ϊ�˾��ȣ��Ŷ���Ϊlong double�ģ�exp(-Lambda)�ǽӽ�0��С��*/
		while (p >= l)
		{
			double u = (double)(rand() % 10000)*0.0001f;
			p *= u;
			k++;
		}
		m_pupr[temp] = k - 1;
		getContext()->m_Config.VeUENum = getContext()->m_Config.VeUENum + k - 1;
	}
	m_Speed = 140;//�����趨,km/h
}


void GTT_HighSpeed::initialize() {
	//��ʼ��m_eNBAry
	m_eNBAry = new GTT_eNB*[getContext()->m_Config.eNBNum];
	eNBConfig _eNBConfig;
	for (int eNBId = 0; eNBId != getContext()->m_Config.eNBNum; ++eNBId) {
		_eNBConfig.eNBId = eNBId;
		m_eNBAry[eNBId] = new GTT_HighSpeed_eNB();
		m_eNBAry[eNBId]->initialize(_eNBConfig);
	}

	//��ʼ��m_RoadAry
	m_RoadAry = new GTT_Road*[m_HighSpeedRodeNum];
	HighSpeedRodeConfig highSpeedRodeConfig;
	for (int roadId = 0; roadId != m_HighSpeedRodeNum; ++roadId) {
		highSpeedRodeConfig.roadId = roadId;
		m_RoadAry[roadId] = new GTT_HighSpeed_Road(highSpeedRodeConfig);
	}

	
	//��ʼ��m_RSUAry
	m_RSUAry = new GTT_RSU*[getContext()->m_Config.RSUNum];
	for (int RSUId = 0; RSUId != getContext()->m_Config.RSUNum; RSUId++) {
		m_RSUAry[RSUId] = new GTT_HighSpeed_RSU();
	}

	//��ʼ��m_VeUEAry
	m_VeUEAry = new GTT_VeUE*[getContext()->m_Config.VeUENum];
	VeUEConfig _VeUEConfig;
	int VeUEId = 0;

	for (int roadId = 0; roadId != m_HighSpeedRodeNum; roadId++) {
		for (int uprIdx = 0; uprIdx != m_pupr[roadId]; uprIdx++) {
			_VeUEConfig.roadId = roadId;
			_VeUEConfig.X = -1732 + rand() % 3465;
			_VeUEConfig.Y = 0.0f;
			_VeUEConfig.AbsX = m_RoadAry[roadId]->m_AbsX + _VeUEConfig.X;
			_VeUEConfig.AbsY = m_RoadAry[roadId]->m_AbsY + _VeUEConfig.Y;
			_VeUEConfig.V = m_Speed;
			_VeUEConfig.VeUENum = getContext()->m_Config.VeUENum;
			m_VeUEAry[VeUEId++] = new GTT_HighSpeed_VeUE(_VeUEConfig);
		}
	}

	for (int VeIdx = 0; VeIdx != getContext()->m_Config.VeUENum; VeIdx++) {
		m_VeUEAry[VeIdx]->m_Distance = new double[gc_RSUNumber];
		for (int RSUIdx = 0; RSUIdx != gc_RSUNumber; RSUIdx++) {
			m_VeUEAry[VeIdx]->m_Distance[RSUIdx] = sqrt(pow((m_VeUEAry[VeIdx]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[VeIdx]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
		}
	}
}


void GTT_HighSpeed::cleanWhenLocationUpdate() {
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		for (auto &c : m_VeUEAry[VeUEId]->m_InterferenceH) {
			if (c != nullptr)
				Delete::safeDelete(c, true);
		}
	}
}


void GTT_HighSpeed::channelGeneration() {
	//RSU.m_VeUEIdList����freshLoc���������ɵģ������Ҫ�ڸ���λ��ǰ�������б�
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		GTT_RSU *_GTT_RSU = m_RSUAry[RSUId];
		_GTT_RSU->m_VeUEIdList.clear();
		for (int clusterIdx = 0; clusterIdx < _GTT_RSU->m_ClusterNum; clusterIdx++) {
			_GTT_RSU->m_ClusterVeUEIdList[clusterIdx].clear();
		}
	}
	//ͬʱҲ���eNB.m_VeUEIdList
	for (int eNBId = 0; eNBId < getContext()->m_Config.eNBNum; eNBId++)
		m_eNBAry[eNBId]->m_VeUEIdList.clear();

	//�˶�ģ��
	freshLoc();

	//�����º��RSU.m_VeUEIdListѹ���Ӧ�Ĵ���
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		GTT_RSU *_GTT_RSU = m_RSUAry[RSUId];
		for (int VeUEId : _GTT_RSU->m_VeUEIdList) {
			int clusterIdx = m_VeUEAry[VeUEId]->m_ClusterIdx;
			_GTT_RSU->m_ClusterVeUEIdList[clusterIdx].push_back(VeUEId);
		}
	}

	//��¼������ÿ������λ����־
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++)
		m_VeUEAry[VeUEId]->m_LocationUpdateLogInfoList.push_back(tuple<int, int>(m_VeUEAry[VeUEId]->m_RSUId, m_VeUEAry[VeUEId]->m_ClusterIdx));

	//��¼RSU�ڳ�������Ŀ
	vector<int> curVeUENum;
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		GTT_RSU *_GTT_RSU = m_RSUAry[RSUId];
		curVeUENum.push_back(static_cast<int>(_GTT_RSU->m_VeUEIdList.size()));
	}
	m_VeUENumPerRSU.push_back(curVeUENum);


	//<UNDONE>:��վ���RSUIDList������ά����?
	//���»�վ��VeUE����
	for (int eNBId = 0; eNBId < getContext()->m_Config.eNBNum; eNBId++) {
		GTT_eNB *_eNB = m_eNBAry[eNBId];
		for (int RSUId : _eNB->m_RSUIdList) {
			for (int VeUEId : m_RSUAry[RSUId]->m_VeUEIdList) {
				_eNB->m_VeUEIdList.push_back(VeUEId);
			}
		}
	}
}


void GTT_HighSpeed::freshLoc() {
	for (int UserIdx = 0; UserIdx != getContext()->m_Config.VeUENum; UserIdx++)
	{

		if (m_VeUEAry[UserIdx]->m_VAngle == 0)
		{
			m_VeUEAry[UserIdx]->m_ClusterIdx = 0;//�����򶫳����ر��Ϊ0

			if ((m_VeUEAry[UserIdx]->m_AbsX + gc_FreshTime*m_VeUEAry[UserIdx]->m_V)>(gc_Length / 2))
			{
				m_VeUEAry[UserIdx]->m_AbsX = (m_VeUEAry[UserIdx]->m_AbsX + gc_FreshTime*m_VeUEAry[UserIdx]->m_V) - gc_Length;
				m_VeUEAry[UserIdx]->m_X = m_VeUEAry[UserIdx]->m_AbsX;
			}
			else
			{
				m_VeUEAry[UserIdx]->m_AbsX = m_VeUEAry[UserIdx]->m_AbsX + gc_FreshTime*m_VeUEAry[UserIdx]->m_V;
				m_VeUEAry[UserIdx]->m_X = m_VeUEAry[UserIdx]->m_AbsX;
			}
		}
		else
		{
			m_VeUEAry[UserIdx]->m_ClusterIdx = 1;//�ɶ����������ر��Ϊ1

			if ((m_VeUEAry[UserIdx]->m_AbsX - gc_FreshTime*m_VeUEAry[UserIdx]->m_V)<(-gc_Length / 2))
			{
				m_VeUEAry[UserIdx]->m_AbsX = (m_VeUEAry[UserIdx]->m_AbsX - gc_FreshTime*m_VeUEAry[UserIdx]->m_V) + gc_Length;
				m_VeUEAry[UserIdx]->m_X = m_VeUEAry[UserIdx]->m_AbsX;
			}
			else
			{
				m_VeUEAry[UserIdx]->m_AbsX = m_VeUEAry[UserIdx]->m_AbsX - gc_FreshTime*m_VeUEAry[UserIdx]->m_V;
				m_VeUEAry[UserIdx]->m_X = m_VeUEAry[UserIdx]->m_AbsX;
			}
		}

		for (int RSUIdx = 0; RSUIdx != gc_RSUNumber; RSUIdx++) {
			m_VeUEAry[UserIdx]->m_Distance[RSUIdx] = sqrt(pow((m_VeUEAry[UserIdx]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[UserIdx]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
		}
	}


	Location location;
	Antenna antenna;
	location.eNBAntH = 5;
	location.VeUEAntH = 1.5;
	location.RSUAntH = 5;

	int RSUIdx = 0;
	for (int UserIdx1 = 0; UserIdx1 != getContext()->m_Config.VeUENum; UserIdx1++)
	{
		//���㳵��������RSU֮���·�����
		double wPL[gc_RSUNumber] = { 0 };
		for (int RSUIdx = 0; RSUIdx != gc_RSUNumber; RSUIdx++) {
			double wSFSTD = 0;
			double wDistanceBP = 4 * (location.VeUEAntH - 1)*(location.RSUAntH - 1)*gc_FC / gc_C;
			if (m_VeUEAry[UserIdx1]->m_Distance[RSUIdx] > 3 && m_VeUEAry[UserIdx1]->m_Distance[RSUIdx] < wDistanceBP)
			{
				wPL[RSUIdx] = 22.7f * log10(m_VeUEAry[UserIdx1]->m_Distance[RSUIdx]) + 27.0f + 20.0f * (log10(gc_FC) - 9.0f);//ת��ΪGHz
			}
			else
			{
				if (wDistanceBP < m_VeUEAry[UserIdx1]->m_Distance[RSUIdx] && m_VeUEAry[UserIdx1]->m_Distance[RSUIdx] < 5000)
				{
					wPL[RSUIdx] = 40.0f * log10(m_VeUEAry[UserIdx1]->m_Distance[RSUIdx]) + 7.56f - 17.3f * log10(location.VeUEAntH - 1) - 17.3f * log10(location.RSUAntH - 1) + 2.7f *(log10(gc_FC) - 9.0f);
				}
				else if (m_VeUEAry[UserIdx1]->m_Distance[RSUIdx] < 3)
				{
					wPL[RSUIdx] = 22.7f * log10(3) + 27.0f + 20.0f * (log10(gc_FC) - 9.0f);
				}
			}
		}

		//���㳵��������RSU֮�����Ӱ˥��
		double wShadow[gc_RSUNumber] = { 0 };
		randomGaussian(wShadow, gc_RSUNumber, 0.0f, 3.0f);

		//���㳵��������RSU֮��Ĵ��г߶�˥���
		double wPLSF[gc_RSUNumber];
		for (int RSUIdx = 0; RSUIdx != gc_RSUNumber; RSUIdx++) {
			wPLSF[RSUIdx] = -(wPL[RSUIdx] + wShadow[RSUIdx]);
		}

		//�������С�Ĵ��г߶�˥��
		int FirstRSUIdx, SecondRSUIdx;
		selectMax(wPLSF, gc_RSUNumber, &FirstRSUIdx, &SecondRSUIdx);
		//����ѡ����С˥���RSU��֮ͨ��
		RSUIdx = FirstRSUIdx;

		m_VeUEAry[UserIdx1]->m_IMTA = new IMTA[getContext()->m_Config.RSUNum];

		m_VeUEAry[UserIdx1]->m_RSUId = RSUIdx;
		m_RSUAry[RSUIdx]->m_VeUEIdList.push_back(UserIdx1);

		//���VeUE��Ϣ���ĵ�
		g_FileVeUEMessage << UserIdx1 << " ";
		g_FileVeUEMessage << m_VeUEAry[UserIdx1]->m_RSUId << " ";
		g_FileVeUEMessage << m_VeUEAry[UserIdx1]->m_ClusterIdx << " ";
		g_FileVeUEMessage << m_VeUEAry[UserIdx1]->m_AbsX << " ";
		g_FileVeUEMessage << m_VeUEAry[UserIdx1]->m_AbsY << endl;

		location.locationType = None;
		location.distance = 0;
		location.distance1 = 0;
		location.distance2 = 0;

		double angle = 0;
		location.manhattan = false;

		location.locationType = Los;
		location.distance = sqrt(pow((m_VeUEAry[UserIdx1]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[UserIdx1]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
		angle = atan2(m_VeUEAry[UserIdx1]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY, m_VeUEAry[UserIdx1]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX) / gc_Degree2PI;

		randomGaussian(location.posCor, 5, 0.0f, 1.0f);//������˹�������Ϊ�����ŵ�ϵ��ʹ�á�

		antenna.TxAngle = angle - m_VeUEAry[UserIdx1]->m_FantennaAngle;
		antenna.RxAngle = angle - m_RSUAry[RSUIdx]->m_FantennaAngle;
		antenna.antGain = 3;
		antenna.byTxAntNum = 1;
		antenna.byRxAntNum = 2;
		antenna.TxSlantAngle = new double[antenna.byTxAntNum];
		antenna.TxAntSpacing = new double[antenna.byTxAntNum];
		antenna.RxSlantAngle = new double[antenna.byRxAntNum];
		antenna.RxAntSpacing = new double[antenna.byRxAntNum];
		antenna.TxSlantAngle[0] = 90.0f;
		antenna.TxAntSpacing[0] = 0.0f;
		antenna.RxSlantAngle[0] = 90.0f;
		antenna.RxSlantAngle[1] = 90.0f;
		antenna.RxAntSpacing[0] = 0.0f;
		antenna.RxAntSpacing[1] = 0.5f;

		double t_Pl = 0;

		m_VeUEAry[UserIdx1]->m_IMTA[RSUIdx].build(&t_Pl, gc_FC, location, antenna, m_VeUEAry[UserIdx1]->m_V*3.6, m_VeUEAry[UserIdx1]->m_VAngle);//�����˽�������ŵ�ģ�ͼ���UE֮���ŵ�ϵ��
		bool *flag = new bool();

		m_VeUEAry[UserIdx1]->m_Ploss = t_Pl;

		*flag = true;
		m_VeUEAry[UserIdx1]->m_IMTA[RSUIdx].enable(flag);

		double *H = new double[1 * 2 * 19 * 2];
		double *FFT = new double[1 * 2 * 1024 * 2];
		double *ch_buffer = new double[1 * 2 * 19 * 20];
		double *ch_sin = new double[1 * 2 * 19 * 20];
		double *ch_cos = new double[1 * 2 * 19 * 20];

		double *t_HAfterFFT = new double[2 * 1024 * 2];

		m_VeUEAry[UserIdx1]->m_IMTA[RSUIdx].calculate(t_HAfterFFT, 0.01f, ch_buffer, ch_sin, ch_cos, H, FFT);
		memcpy(m_VeUEAry[UserIdx1]->m_H, t_HAfterFFT, 2 * 1024 * 2 * sizeof(double(0)));
	

		Delete::safeDelete(flag);
		Delete::safeDelete(H, true);
		Delete::safeDelete(ch_buffer, true);
		Delete::safeDelete(ch_sin, true);
		Delete::safeDelete(ch_cos, true);
		Delete::safeDelete(antenna.TxSlantAngle, true);
		Delete::safeDelete(antenna.TxAntSpacing, true);
		Delete::safeDelete(antenna.RxSlantAngle, true);
		Delete::safeDelete(antenna.RxAntSpacing, true);
		Delete::safeDelete(m_VeUEAry[UserIdx1]->m_IMTA, true);
		Delete::safeDelete(FFT, true);
		Delete::safeDelete(t_HAfterFFT, true);
	}
}


void GTT_HighSpeed::writeVeUELocationUpdateLogInfo(ofstream &out1, ofstream &out2) {
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		out1 << "VeUE[ " << left << setw(3) << VeUEId << "]" << endl;
		out1 << "{" << endl;
		for (const tuple<int, int> &t : m_VeUEAry[VeUEId]->m_LocationUpdateLogInfoList)
			out1 << "    " << "[ RSUId = " << left << setw(2) << get<0>(t) << " , ClusterIdx = " << get<1>(t) << " ]" << endl;
		out1 << "}" << endl;
	}
	for (const vector<int> &v : m_VeUENumPerRSU) {
		for (int i : v) {
			out2 << i << " ";
		}
		out2 << endl;
	}
}


void GTT_HighSpeed::calculateInterference(const vector<vector<list<int>>>& t_RRMInterferenceVec) {
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		m_VeUEAry[VeUEId]->m_IMTA = new IMTA[getContext()->m_Config.RSUNum];
	}

	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		for (int patternIdx = 0; patternIdx < t_RRMInterferenceVec[VeUEId].size(); patternIdx++) {
			const list<int> &lst = t_RRMInterferenceVec[VeUEId][patternIdx];//��ǰ��������ǰPattern�����и��ų�����Id

			for (int interferenceVeUEId : lst) {

				if (m_VeUEAry[VeUEId]->m_InterferenceH[interferenceVeUEId] != nullptr) continue;

				m_VeUEAry[VeUEId]->m_InterferenceH[interferenceVeUEId] = new double[2 * 1024 * 2];

				Location location;
				Antenna antenna;


				int RSUIdx = m_VeUEAry[VeUEId]->m_RSUId;
				location.locationType = None;
				location.distance = 0;
				location.distance1 = 0;
				location.distance2 = 0;

				double angle = 0;
				location.manhattan = false;

				location.locationType = Los;
				location.distance = sqrt(pow((m_VeUEAry[interferenceVeUEId]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[interferenceVeUEId]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
				angle = atan2(m_VeUEAry[interferenceVeUEId]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY, m_VeUEAry[interferenceVeUEId]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX) / gc_Degree2PI;

				location.eNBAntH = 5;
				location.VeUEAntH = 1.5;
				randomGaussian(location.posCor, 5, 0.0f, 1.0f);//������˹�������Ϊ�����ŵ�ϵ��ʹ�á�

				antenna.TxAngle = angle - m_VeUEAry[interferenceVeUEId]->m_FantennaAngle;
				antenna.RxAngle = angle - m_RSUAry[RSUIdx]->m_FantennaAngle;
				antenna.antGain = 6;
				antenna.byTxAntNum = 1;
				antenna.byRxAntNum = 2;
				antenna.TxSlantAngle = new double[antenna.byTxAntNum];
				antenna.TxAntSpacing = new double[antenna.byTxAntNum];
				antenna.RxSlantAngle = new double[antenna.byRxAntNum];
				antenna.RxAntSpacing = new double[antenna.byRxAntNum];
				antenna.TxSlantAngle[0] = 90.0f;
				antenna.TxAntSpacing[0] = 0.0f;
				antenna.RxSlantAngle[0] = 90.0f;
				antenna.RxSlantAngle[1] = 90.0f;
				antenna.RxAntSpacing[0] = 0.0f;
				antenna.RxAntSpacing[1] = 0.5f;

				double t_Pl = 0;
				m_VeUEAry[interferenceVeUEId]->m_IMTA[RSUIdx].build(&t_Pl, gc_FC, location, antenna, m_VeUEAry[interferenceVeUEId]->m_V*3.6, m_VeUEAry[interferenceVeUEId]->m_VAngle);//�����˽�������ŵ�ģ�ͼ���UE֮���ŵ�ϵ��
				bool *flag = new bool();


				m_VeUEAry[VeUEId]->m_InterferencePloss[interferenceVeUEId] = t_Pl;


				*flag = true;
				m_VeUEAry[interferenceVeUEId]->m_IMTA[RSUIdx].enable(flag);
				double *H = new double[1 * 2 * 19 * 2];
				double *FFT = new double[1 * 2 * 1024 * 2];
				double *ch_buffer = new double[1 * 2 * 19 * 20];
				double *ch_sin = new double[1 * 2 * 19 * 20];
				double *ch_cos = new double[1 * 2 * 19 * 20];

				double *t_HAfterFFT = new double[2 * 1024 * 2];

				m_VeUEAry[interferenceVeUEId]->m_IMTA[RSUIdx].calculate(t_HAfterFFT, 0.01f, ch_buffer, ch_sin, ch_cos, H, FFT);


				memcpy(m_VeUEAry[VeUEId]->m_InterferenceH[interferenceVeUEId], t_HAfterFFT, 2 * 1024 * 2 * sizeof(double(0)));

				Delete::safeDelete(flag);
				Delete::safeDelete(H, true);
				Delete::safeDelete(ch_buffer, true);
				Delete::safeDelete(ch_sin, true);
				Delete::safeDelete(ch_cos, true);
				Delete::safeDelete(antenna.TxSlantAngle, true);
				Delete::safeDelete(antenna.TxAntSpacing, true);
				Delete::safeDelete(antenna.RxSlantAngle, true);
				Delete::safeDelete(antenna.RxAntSpacing, true);
				Delete::safeDelete(FFT, true);
				Delete::safeDelete(t_HAfterFFT, true);
			}
		}
	}

	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		Delete::safeDelete(m_VeUEAry[VeUEId]->m_IMTA, true);
	}
}