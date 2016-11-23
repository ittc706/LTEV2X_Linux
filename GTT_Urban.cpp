/*
* =====================================================================================
*
*       Filename:  GTT_Urban.cpp
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
#include"GTT_Urban.h"
#include"Exception.h"
#include"Function.h"
#include"System.h"

using namespace std;
using namespace ns_GTT_Urban;


GTT_Urban_VeUE::GTT_Urban_VeUE(VeUEConfig &t_VeUEConfig) {

	m_RoadId = t_VeUEConfig.roadId;
	m_LocationId = t_VeUEConfig.locationId;
	m_X = t_VeUEConfig.X;
	m_Y = t_VeUEConfig.Y;
	m_AbsX = t_VeUEConfig.AbsX;
	m_AbsY = t_VeUEConfig.AbsY;
	m_V = t_VeUEConfig.V;

	if ((0 < m_LocationId) && (m_LocationId <= 61))
		m_VAngle = 90;
	else if ((61 < m_LocationId) && (m_LocationId <= 96))
		m_VAngle = 0;
	else if ((96 < m_LocationId) && (m_LocationId <= 157))
		m_VAngle = -90;
	else
		m_VAngle = -180;

	randomUniform(&m_FantennaAngle, 1, 180.0f, -180.0f, false);

	m_Nt = 1;
	m_Nr = 2;
	m_H = new double[2 * 1024 * 2];
	m_InterferencePloss = vector<double>(t_VeUEConfig.VeUENum, 0);
	m_InterferenceH = vector<double*>(t_VeUEConfig.VeUENum, nullptr);
}


GTT_Urban_RSU::GTT_Urban_RSU() {
	m_AbsX = ns_GTT_Urban::gc_RSUTopoRatio[m_RSUId * 2 + 0] * ns_GTT_Urban::gc_Width;
	m_AbsY = ns_GTT_Urban::gc_RSUTopoRatio[m_RSUId * 2 + 1] * ns_GTT_Urban::gc_Length;
	randomUniform(&m_FantennaAngle, 1, 180.0f, -180.0f, false);
	g_FileLocationInfo << toString(0);

	m_ClusterNum = ns_GTT_Urban::gc_RSUClusterNum[m_RSUId];
	m_ClusterVeUEIdList = vector<list<int>>(m_ClusterNum);
}


void GTT_Urban_eNB::initialize(eNBConfig &t_eNBConfig) {
	m_RoadId = t_eNBConfig.roadId;
	m_eNBId = t_eNBConfig.eNBId;
	m_X = t_eNBConfig.X;
	m_Y = t_eNBConfig.Y;
	m_AbsX = t_eNBConfig.AbsX;
	m_AbsY = t_eNBConfig.AbsY;
	g_FileLocationInfo << toString(0);
}


GTT_Urban_Road::GTT_Urban_Road(UrbanRoadConfig &t_RoadConfig) {
	m_RoadId = t_RoadConfig.roadId;
	m_AbsX = ns_GTT_Urban::gc_RoadTopoRatio[m_RoadId * 2 + 0] * ns_GTT_Urban::gc_Width;
	m_AbsY = ns_GTT_Urban::gc_RoadTopoRatio[m_RoadId * 2 + 1] * ns_GTT_Urban::gc_Length;
	g_FileLocationInfo << toString(0);

	m_eNBNum = t_RoadConfig.eNBNum;
	if (m_eNBNum == 1) {
		/*
		* ����Ƚ��ƣ�����һ��
		* ����t_RoadConfig.eNB�����һ��(GTT_eNB**)���ͣ�˫��ָ��
		* ��һ��ָ��ָ������
		*/
		m_eNB = *((GTT_eNB **)t_RoadConfig.eNB + t_RoadConfig.eNBOffset);
		eNBConfig eNBConfig;
		eNBConfig.systemConfig = t_RoadConfig.systemConfig;
		eNBConfig.roadId = m_RoadId;
		eNBConfig.X = 42.0f;
		eNBConfig.Y = -88.5f;
		eNBConfig.AbsX = m_AbsX + eNBConfig.X;
		eNBConfig.AbsY = m_AbsY + eNBConfig.Y;
		eNBConfig.eNBId = t_RoadConfig.eNBOffset;
		m_eNB->initialize(eNBConfig);
	}
}


default_random_engine GTT_Urban::s_Engine((unsigned)time(NULL));

GTT_Urban::GTT_Urban(System* t_Context) :
	GTT(t_Context) {}


void GTT_Urban::configure() {
	getContext()->m_Config.eNBNum = gc_eNBNumber;
	getContext()->m_Config.RoadNum = gc_RoadNumber;
	getContext()->m_Config.RSUNum = gc_RSUNumber;//Ŀǰֻ��ʾUE RSU��
	m_pupr = new int[getContext()->m_Config.RoadNum];
	getContext()->m_Config.VeUENum = 0;
	int Lambda = static_cast<int>((gc_Length + gc_Width) * 2 * 3.6 / (2.5 * 15));
	for (int temp = 0; temp != getContext()->m_Config.RoadNum; ++temp)
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
	m_xNum = 36;
	m_yNum = 62;
	m_ueTopoNum = (m_xNum + m_yNum) * 2 - 4;
	m_pueTopo = new double[m_ueTopoNum * 2];//�غ���4��
	double temp_x = -(double)gc_Width / 2 + gc_LaneWidth;
	double temp_y = -(double)gc_Length / 2 + gc_LaneWidth;
	for (int temp = 0; temp != m_ueTopoNum; ++temp)
	{
		if (temp>0 && temp <= 61) {
			if (temp == 60) temp_y += 6; else temp_y += 7;
		}
		else if (61<temp&&temp <= 96) {
			if (temp == 95) temp_x += 5; else temp_x += 7;
		}
		else if (96<temp&&temp <= 157) {
			if (temp == 156) temp_y -= 6; else temp_y -= 7;
		}
		else if (157<temp&&temp <= 192) {
			if (temp == 191) temp_x -= 5; else temp_x -= 7;
		}

		m_pueTopo[temp * 2 + 0] = temp_x;
		m_pueTopo[temp * 2 + 1] = temp_y;
	}
	m_Speed = 15;//�����趨,km/h
}


void GTT_Urban::initialize() {
	//��ʼ��m_eNBAry
	m_eNBAry = new GTT_eNB*[getContext()->m_Config.eNBNum];
	for (int eNBId = 0; eNBId < getContext()->m_Config.eNBNum; eNBId++) {
		m_eNBAry[eNBId] = new GTT_Urban_eNB();
		//��Road��ʼ����ʱ�򣬶�eNB���г�ʼ��
	}
	
	//��ʼ��m_RoadAry
	m_RoadAry = new GTT_Road*[getContext()->m_Config.RoadNum];
	UrbanRoadConfig urbanRoadConfig;
	for (int roadId = 0; roadId != getContext()->m_Config.RoadNum; ++roadId) {
		urbanRoadConfig.roadId = roadId;
		if (roadId % 2 == 0) {
			urbanRoadConfig.eNBNum = 1;
			urbanRoadConfig.eNB = m_eNBAry;
			urbanRoadConfig.eNBOffset = roadId / 2;
		}
		else {
			urbanRoadConfig.eNBNum = 0;
		}
		m_RoadAry[roadId] = new GTT_Urban_Road(urbanRoadConfig);
	}

	//��ʼ��m_RSUAry
	m_RSUAry = new GTT_RSU*[getContext()->m_Config.RSUNum];
	for (int RSUId = 0; RSUId != getContext()->m_Config.RSUNum; RSUId++) {
		m_RSUAry[RSUId] = new GTT_Urban_RSU();
	}

	//��ʼ��m_VeUEAry
	m_VeUEAry = new GTT_VeUE*[getContext()->m_Config.VeUENum];
	VeUEConfig _VeUEConfig;
	int VeUEId = 0;

	for (int RoadIdx = 0; RoadIdx != getContext()->m_Config.RoadNum; RoadIdx++) {
		for (int uprIdx = 0; uprIdx != m_pupr[RoadIdx]; uprIdx++) {
			_VeUEConfig.roadId = RoadIdx;
			_VeUEConfig.locationId = rand() % m_ueTopoNum;
			_VeUEConfig.X = m_pueTopo[_VeUEConfig.locationId * 2 + 0];
			_VeUEConfig.Y = m_pueTopo[_VeUEConfig.locationId * 2 + 1];
			_VeUEConfig.AbsX = m_RoadAry[RoadIdx]->m_AbsX + _VeUEConfig.X;
			_VeUEConfig.AbsY = m_RoadAry[RoadIdx]->m_AbsY + _VeUEConfig.Y;
			_VeUEConfig.V = m_Speed;
			_VeUEConfig.VeUENum = getContext()->m_Config.VeUENum;
			m_VeUEAry[VeUEId++] = new GTT_Urban_VeUE(_VeUEConfig);
		}
	}
	//��ʼ��������RSU�ľ���
	for (int VeIdx = 0; VeIdx != getContext()->m_Config.VeUENum; VeIdx++) {
		m_VeUEAry[VeIdx]->m_Distance = new double[gc_RSUNumber];
			for (int RSUIdx = 0; RSUIdx != gc_RSUNumber; RSUIdx++) {
				m_VeUEAry[VeIdx]->m_Distance[RSUIdx] = sqrt(pow((m_VeUEAry[VeIdx]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[VeIdx]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
			}
	}
}


void GTT_Urban::cleanWhenLocationUpdate() {
	for (int VeUEId = 0; VeUEId <getContext()->m_Config.VeUENum; VeUEId++) {
		for (auto &c : m_VeUEAry[VeUEId]->m_InterferenceH) {
			if (c != nullptr)
				Delete::safeDelete(c, true);
		}
	}
}


void GTT_Urban::channelGeneration() {
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
	for (int RSUId = 0; RSUId <getContext()->m_Config.RSUNum; RSUId++) {
		GTT_RSU *_GTT_RSU = m_RSUAry[RSUId];
		for (int VeUEId : _GTT_RSU->m_VeUEIdList) {
			int clusterIdx = m_VeUEAry[VeUEId]->m_ClusterIdx;
			_GTT_RSU->m_ClusterVeUEIdList[clusterIdx].push_back(VeUEId);
		}
	}

	//��¼������ÿ������λ����־
	for (int VeUEId = 0; VeUEId<getContext()->m_Config.VeUENum; VeUEId++)
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
	for (int eNBId = 0; eNBId <getContext()->m_Config.eNBNum; eNBId++) {
		GTT_eNB *_eNB = m_eNBAry[eNBId];
		for (int RSUId : _eNB->m_RSUIdList) {
			for (int VeUEId : m_RSUAry[RSUId]->m_VeUEIdList) {
				_eNB->m_VeUEIdList.push_back(VeUEId);
			}
		}
	}
}


void GTT_Urban::freshLoc() {
	for (int UserIdx = 0; UserIdx != getContext()->m_Config.VeUENum; UserIdx++)
	{
		bool RoadChangeFlag = false;
		int temp;
		switch (m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId)
		{
		case 191:
			m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 0;
			break;
		case 0:
			temp = rand() % 4;
			if (temp == 0)//turn left
			{
				RoadChangeFlag = true;
				m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][4];
				m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 96;
				m_VeUEAry[UserIdx]->m_VAngle = -90;
			}
			else
				if (temp == 2)//turn right
				{
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId + 1;
					m_VeUEAry[UserIdx]->m_VAngle = 90;
				}
				else//go straight�����ı䷽��
				{
					RoadChangeFlag = true;
					m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][5];
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 157;
				}
			break;
		case 61:
			temp = rand() % 4;
			if (temp == 0)//turn left
			{
				RoadChangeFlag = true;
				m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][6];
				m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 157;
				m_VeUEAry[UserIdx]->m_VAngle = -180;
			}
			else
				if (temp == 2)
				{
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId + 1;
					m_VeUEAry[UserIdx]->m_VAngle = 0;
				}
				else
				{
					RoadChangeFlag = true;
					m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][7];
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 0;
					m_VeUEAry[UserIdx]->m_VAngle = 270;
				}
			break;
		case 96:
			temp = rand() % 4;
			if (temp == 0)//turn left
			{
				RoadChangeFlag = true;
				m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][8];
				m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 0;
				m_VeUEAry[UserIdx]->m_VAngle = 90;
			}
			else
				if (temp == 2)
				{
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId + 1;
					m_VeUEAry[UserIdx]->m_VAngle = -90;
				}
				else
				{
					RoadChangeFlag = true;
					m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][1];
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 61;
				}
			break;
		case 157:
			temp = rand() % 4;
			if (temp == 0)//turn left
			{
				RoadChangeFlag = true;
				m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][2];
				m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 61;
				m_VeUEAry[UserIdx]->m_VAngle = 0;
			}
			else
				if (temp == 2)
				{
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId + 1;
					m_VeUEAry[UserIdx]->m_VAngle = -180;
				}
				else
				{
					RoadChangeFlag = true;
					m_VeUEAry[UserIdx]->m_RoadId = gc_WrapAroundRoad[m_VeUEAry[UserIdx]->m_RoadId][3];
					m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = 96;
				}
			break;
		default:
			m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId = m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId + 1;
			break;
		}
		m_VeUEAry[UserIdx]->m_X = m_pueTopo[m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId * 2 + 0];
		m_VeUEAry[UserIdx]->m_Y = m_pueTopo[m_VeUEAry[UserIdx]->getUrbanPoint()->m_LocationId * 2 + 1];
		m_VeUEAry[UserIdx]->m_AbsX = m_RoadAry[m_VeUEAry[UserIdx]->m_RoadId]->m_AbsX + m_VeUEAry[UserIdx]->m_X;
		m_VeUEAry[UserIdx]->m_AbsY = m_RoadAry[m_VeUEAry[UserIdx]->m_RoadId]->m_AbsY + m_VeUEAry[UserIdx]->m_Y;

		//���³���������RSU֮��ľ���
		for (int RSUIdx = 0; RSUIdx != gc_RSUNumber; RSUIdx++) {
			m_VeUEAry[UserIdx]->m_Distance[RSUIdx] = sqrt(pow((m_VeUEAry[UserIdx]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[UserIdx]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
		}
	}


	Location location;
	Antenna antenna;

	location.manhattan = true;

	location.eNBAntH = 5;
	location.VeUEAntH = 1.5;
	location.RSUAntH = 5;

	randomGaussian(location.posCor, 5, 0.0f, 1.0f);//������˹�������Ϊ�����ŵ�ϵ��ʹ�á�

	int RSUIdx = 0;
	int ClusterID = 0;

	for (int UserIdx1 = 0; UserIdx1 != getContext()->m_Config.VeUENum; UserIdx1++)
	{
		//���㳵��������RSU֮���·�����
		double wPL[gc_RSUNumber] = {0};
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
		//�������ΪRSU�յ�Ϊ����������
		double vectorI2V[2];
		vectorI2V[0] = m_VeUEAry[UserIdx1]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX;//����������
		vectorI2V[1] = m_VeUEAry[UserIdx1]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY;//����������
		double tan = vectorI2V[1] / vectorI2V[0];//���㷽���
		//���ݷ�����ж����ڴص�Id
		if (vectorI2V[0] >= 0 && vectorI2V[1] >= 0) {
			if (tan < 1)
				ClusterID = 1;
			else
				ClusterID = 0;
		}
		else if (vectorI2V[0] < 0 && vectorI2V[1] >= 0) {
			if (tan < -1)
				ClusterID = 0;
			else
				ClusterID = 3;
		}
		else if (vectorI2V[0] < 0 && vectorI2V[1] < 0) {
			if (tan < 1)
				ClusterID = 3;
			else
				ClusterID = 2;
		}
		else {
			if (tan < -1)
				ClusterID = 2;
			else
				ClusterID = 1;
		}

		m_VeUEAry[UserIdx1]->m_RSUId = RSUIdx;
		m_VeUEAry[UserIdx1]->m_ClusterIdx = ClusterID;
		m_RSUAry[RSUIdx]->m_VeUEIdList.push_back(UserIdx1);
		location.locationType = None;
		location.distance = 0;
		location.distance1 = 0;
		location.distance2 = 0;

		double angle = 0;
		if (location.manhattan == true)  //����location distance
		{
		
			location.locationType = Los;// ����������Ӧ��RSU֮���ΪLOS
			location.distance = sqrt(pow((m_VeUEAry[UserIdx1]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[UserIdx1]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
			angle = atan2(m_VeUEAry[UserIdx1]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY, m_VeUEAry[UserIdx1]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX) / gc_Degree2PI;

		}
	
		

		antenna.TxAngle = angle - m_VeUEAry[UserIdx1]->m_FantennaAngle;
		antenna.RxAngle = angle - m_RSUAry[RSUIdx]->m_FantennaAngle;
		antenna.antGain = 6;//�շ����߸�3dbi
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

		m_VeUEAry[UserIdx1]->m_IMTA[RSUIdx].build(&t_Pl, gc_FC, location, antenna, m_VeUEAry[UserIdx1]->m_V, m_VeUEAry[UserIdx1]->m_VAngle);//�����˽�������ŵ�ģ�ͼ���UE֮���ŵ�ϵ��
		bool *flag = new bool();

		m_VeUEAry[UserIdx1]->m_Ploss = t_Pl;

		*flag = true;
		m_VeUEAry[UserIdx1]->m_IMTA[RSUIdx].enable(flag);
		double *H = new double[1 * 2 * 12 * 2];
		double *FFT = new double[1 * 2 * 1024 * 2];
		double *ch_buffer = new double[1 * 2 * 12 * 20];
		double *ch_sin = new double[1 * 2 * 12 * 20];
		double *ch_cos = new double[1 * 2 * 12 * 20];

		double *t_HAfterFFT = new double[2 * 1024 * 2];

		m_VeUEAry[UserIdx1]->m_IMTA[RSUIdx].calculate(t_HAfterFFT,0.01f, ch_buffer, ch_sin, ch_cos, H,FFT);
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


void GTT_Urban::writeVeUELocationUpdateLogInfo(ofstream &t_File1, ofstream &t_File2) {
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		t_File1 << "VeUE[ " << left << setw(3) << VeUEId << "]" << endl;
		t_File1 << "{" << endl;
		for (const tuple<int, int> &t : m_VeUEAry[VeUEId]->m_LocationUpdateLogInfoList)
			t_File1 << "    " << "[ RSUId = " << left << setw(2) << get<0>(t) << " , ClusterIdx = " << get<1>(t) << " ]" << endl;
		t_File1 << "}" << endl;
	}
	for (const vector<int> &v : m_VeUENumPerRSU) {
		for (int i : v) {
			t_File2 << i << " ";
		}
		t_File2 << endl;
	}
}

void GTT_Urban::calculateInterference(const vector<vector<list<int>>>& t_RRMInterferenceVec) {
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		m_VeUEAry[VeUEId]->m_IMTA = new IMTA[getContext()->m_Config.RSUNum];
	}

	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		for (int patternIdx = 0; patternIdx < t_RRMInterferenceVec[0].size(); patternIdx++) {
			const list<int> &interList = t_RRMInterferenceVec[VeUEId][patternIdx];//��ǰ��������ǰPattern�����и��ų�����Id

			for (int interferenceVeUEId : interList) {

				if (m_VeUEAry[VeUEId]->m_InterferenceH[interferenceVeUEId] != nullptr) continue;

				m_VeUEAry[VeUEId]->m_InterferenceH[interferenceVeUEId] = new double[2 * 1024 * 2];

				Location location;
				Antenna antenna;
				location.manhattan = true;

				int RSUIdx = m_VeUEAry[VeUEId]->m_RSUId;
				location.locationType = None;
				location.distance = 0;
				location.distance1 = 0;
				location.distance2 = 0;

				double angle = 0;
				if (location.manhattan == true)  //����location distance
				{
					if (abs(m_VeUEAry[interferenceVeUEId]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX) <= 10.5 || abs(m_VeUEAry[interferenceVeUEId]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY) <= 10.5)
					{
						location.locationType = Los;
						location.distance = sqrt(pow((m_VeUEAry[interferenceVeUEId]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX), 2.0f) + pow((m_VeUEAry[interferenceVeUEId]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY), 2.0f));
						angle = atan2(m_VeUEAry[interferenceVeUEId]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY, m_VeUEAry[interferenceVeUEId]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX) / gc_Degree2PI;
					}
					else
					{
						location.locationType = Nlos;
						location.distance1 = abs(m_VeUEAry[interferenceVeUEId]->m_AbsX - m_RSUAry[RSUIdx]->m_AbsX);
						location.distance2 = abs(m_VeUEAry[interferenceVeUEId]->m_AbsY - m_RSUAry[RSUIdx]->m_AbsY);
						location.distance = sqrt(pow(location.distance1, 2.0f) + pow(location.distance2, 2.0f));

					}
				}
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
				m_VeUEAry[interferenceVeUEId]->m_IMTA[RSUIdx].build(&t_Pl, gc_FC, location, antenna, m_VeUEAry[interferenceVeUEId]->m_V, m_VeUEAry[interferenceVeUEId]->m_VAngle);//�����˽�������ŵ�ģ�ͼ���UE֮���ŵ�ϵ��
				bool *flag = new bool();


				m_VeUEAry[VeUEId]->m_InterferencePloss[interferenceVeUEId] = t_Pl;


				*flag = true;
				m_VeUEAry[interferenceVeUEId]->m_IMTA[RSUIdx].enable(flag);
				double *H = new double[1 * 2 * 12 * 2];
				double *FFT = new double[1 * 2 * 1024 * 2];
				double *ch_buffer = new double[1 * 2 * 12 * 20];
				double *ch_sin = new double[1 * 2 * 12 * 20];
				double *ch_cos = new double[1 * 2 * 12 * 20];

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