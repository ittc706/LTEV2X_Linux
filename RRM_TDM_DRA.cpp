/*
* =====================================================================================
*
*       Filename:  RRM_TDM_DRA.cpp
*
*    Description:  RRM_TDM_DRAģ��
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

#include<tuple>
#include<vector>
#include<list>
#include<sstream>
#include<iomanip>
#include<set>
#include"RRM_TDM_DRA.h"
#include"Exception.h"
#include"Function.h"
#include"System.h"

using namespace std;


default_random_engine RRM_TDM_DRA_VeUE::s_Engine((unsigned)time(NULL));


RRM_TDM_DRA_VeUE::RRM_TDM_DRA_VeUE() :RRM_VeUE(ns_RRM_TDM_DRA::gc_TotalPatternNum) {}


string RRM_TDM_DRA_VeUE::toString(int t_NumTab) {
	string indent;
	for (int i = 0; i < t_NumTab; i++)
		indent.append("    ");

	ostringstream ss;
	ss << indent << "{ VeUEId = " << left << setw(3) << getSystemPoint()->getGTTPoint()->m_VeUEId;
	ss << " , RSUId = " << left << setw(3) << getSystemPoint()->getGTTPoint()->m_RSUId;
	ss << " , ClusterIdx = " << left << setw(3) << getSystemPoint()->getGTTPoint()->m_ClusterIdx;
	ss << " , ScheduleInterval = [" << left << setw(3) << get<0>(m_ScheduleInterval) << "," << left << setw(3) << get<1>(m_ScheduleInterval) << "] }";
	return ss.str();
}


RRM_TDM_DRA_RSU::RRM_TDM_DRA_RSU() {}


void RRM_TDM_DRA_RSU::initialize() {
	m_AccessEventIdList = vector<pair<list<int>, list<int>>>(getSystemPoint()->getGTTPoint()->m_ClusterNum);
	m_WaitEventIdList = vector<pair<list<int>, list<int>>>(getSystemPoint()->getGTTPoint()->m_ClusterNum);
	m_PatternIsAvailable = vector<vector<bool>>(getSystemPoint()->getGTTPoint()->m_ClusterNum, vector<bool>(ns_RRM_TDM_DRA::gc_TotalPatternNum, true));
	m_ScheduleInfoTable = vector<vector<ScheduleInfo*>>(getSystemPoint()->getGTTPoint()->m_ClusterNum, vector<ScheduleInfo*>(ns_RRM_TDM_DRA::gc_TotalPatternNum, nullptr));
	m_TransimitScheduleInfoList = vector<vector<list<ScheduleInfo*>>>(getSystemPoint()->getGTTPoint()->m_ClusterNum, vector<list<ScheduleInfo*>>(ns_RRM_TDM_DRA::gc_TotalPatternNum));
}


int RRM_TDM_DRA_RSU::getClusterIdx(int t_TTI) {
	int roundATTI = t_TTI%ns_RRM_TDM_DRA::gc_NTTI; //��TTIӳ�䵽[0-gc_DRA_NTTI)�ķ�Χ
	for (int clusterIdx = 0; clusterIdx < getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++)
		if (roundATTI <= get<1>(m_ClusterTDRInfo[clusterIdx])) return clusterIdx;
	return -1;
}


string RRM_TDM_DRA_RSU::toString(int t_NumTab) {
	string indent;
	for (int i = 0; i < t_NumTab; i++)
		indent.append("    ");

	ostringstream ss;
	//������Ϣ
	ss << indent << "RSU[" << getSystemPoint()->getGTTPoint()->m_RSUId << "] :" << endl;
	ss << indent << "{" << endl;

	//��ʼ��ӡVeUEIdList
	ss << indent << "    " << "VeUEIdList :" << endl;
	ss << indent << "    " << "{" << endl;
	for (int clusterIdx = 0; clusterIdx < getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
		ss << indent << "        " << "Cluster[" << clusterIdx << "] :" << endl;
		ss << indent << "        " << "{" << endl;
		int cnt = 0;
		for (int RSUId : getSystemPoint()->getGTTPoint()->m_ClusterVeUEIdList[clusterIdx]) {
			if (cnt % 10 == 0)
				ss << indent << "            [ ";
			ss << left << setw(3) << RSUId << " , ";
			if (cnt % 10 == 9)
				ss << " ]" << endl;
			cnt++;
		}
		if (cnt != 0 && cnt % 10 != 0)
			ss << " ]" << endl;
		ss << indent << "        " << "}" << endl;
	}
	ss << indent << "    " << "}" << endl;

	//��ʼ��ӡclusterInfo
	ss << indent << "    " << "clusterInfo :" << endl;
	ss << indent << "    " << "{" << endl;

	ss << indent << "        ";
	for (const tuple<int, int, int>&t : getTDM_DRAPoint()->m_ClusterTDRInfo)
		ss << "[ " << get<0>(t) << " , " << get<1>(t) << " ] ,";
	ss << endl;
	ss << indent << "    " << "}" << endl;


	//������Ϣ
	ss << indent << "}" << endl;
	return ss.str();
}



RRM_TDM_DRA::RRM_TDM_DRA(System* t_Context) :
	RRM(t_Context) {
	m_ThreadNum = t_Context->m_Config.threadNum;

	m_InterferenceVec = vector<vector<list<int>>>(getContext()->m_Config.VeUENum, vector<list<int>>(ns_RRM_TDM_DRA::gc_TotalPatternNum));
	m_ThreadsRSUIdRange = vector<pair<int, int>>(m_ThreadNum);

	int num = getContext()->m_Config.RSUNum / m_ThreadNum;
	for (int threadIdx = 0; threadIdx < m_ThreadNum; threadIdx++) {
		m_ThreadsRSUIdRange[threadIdx] = pair<int, int>(threadIdx*num, (threadIdx + 1)*num - 1);
	}
	m_ThreadsRSUIdRange[m_ThreadNum - 1].second = getContext()->m_Config.RSUNum - 1;//�������һ���߽�
}


void RRM_TDM_DRA::initialize() {
	//��ʼ��VeUE�ĸ�ģ���������
	m_VeUEAry = new RRM_VeUE*[getContext()->m_Config.VeUENum];
	for (int VeUEId = 0; VeUEId <getContext()->m_Config.VeUENum; VeUEId++) {
		m_VeUEAry[VeUEId] = new RRM_TDM_DRA_VeUE();
	}

	//��ʼ��RSU�ĸ�ģ���������
	m_RSUAry = new RRM_RSU*[getContext()->m_Config.RSUNum];
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		m_RSUAry[RSUId] = new RRM_TDM_DRA_RSU();
	}
}


void RRM_TDM_DRA::cleanWhenLocationUpdate() {
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		for (vector<int>& preInterferenceVeUEIdVec : m_VeUEAry[VeUEId]->m_PreInterferenceVeUEIdVec)
			preInterferenceVeUEIdVec.clear();

		m_VeUEAry[VeUEId]->m_PreSINR.assign(ns_RRM_TDM_DRA::gc_TotalPatternNum, (numeric_limits<double>::min)());
	}
}


void RRM_TDM_DRA::schedule() {
	bool clusterFlag = getContext()->m_TTI  %getContext()->m_Config.locationUpdateNTTI == 0;

	//��Դ������Ϣ���:����ÿ��RSU�ڵĽ��������
	informationClean();

	//���ݴش�С����ʱ����Դ�Ļ���
	//groupSizeBasedTDM(clusterFlag);
	uniformTDM(clusterFlag);

	//���µȴ�����
	updateAccessEventIdList(clusterFlag);

	//��Դѡ��
	selectRBBasedOnP123();

	//ͳ��ʱ����Ϣ
	delaystatistics();

	//֡����ͻ
	conflictListener();

	//����������˵�Ԫ���������Ӧ����
	transimitPreparation();

	//ģ�⴫�俪ʼ�����µ�����Ϣ
	transimitStart();

	//д�������Ϣ
	writeScheduleInfo(g_FileScheduleInfo);

	//ģ�⴫�������ͳ��������
	transimitEnd();
}


void RRM_TDM_DRA::informationClean() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			_RSU->getTDM_DRAPoint()->m_AccessEventIdList[clusterIdx].first.clear();
			_RSU->getTDM_DRAPoint()->m_AccessEventIdList[clusterIdx].second.clear();
		}
	}
}


void RRM_TDM_DRA::groupSizeBasedTDM(bool t_ClusterFlag) {
	if (!t_ClusterFlag)return;
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		//�������������RSU����һ����ʱ
		if (_RSU->getSystemPoint()->getGTTPoint()->m_VeUEIdList.size() == 0) {
			/*-----------------------ATTENTION-----------------------
			* ����ֵΪ(0,-1,0)�ᵼ�»�ȡ��ǰ�ر��ʧ�ܣ����������ط���Ҫ����
			* ���ֱ�Ӹ�ÿ���ض���ֵΪ�������䣬����Ҳû���κ����ã����������������
			*------------------------ATTENTION-----------------------*/
			_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo = vector<tuple<int, int, int>>(_RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum, tuple<int, int, int>(0, ns_RRM_TDM_DRA::gc_NTTI - 1, ns_RRM_TDM_DRA::gc_NTTI));
			continue;
		}

		//��ʼ��
		_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo = vector<tuple<int, int, int>>(_RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum, tuple<int, int, int>(0, 0, 0));

		//����ÿ��TTIʱ϶��Ӧ��VeUE��Ŀ(double)��!!!������!!��
		double VeUESizePerTTI = static_cast<double>(_RSU->getSystemPoint()->getGTTPoint()->m_VeUEIdList.size()) / static_cast<double>(ns_RRM_TDM_DRA::gc_NTTI);

		//clusterSize�洢ÿ���ص�VeUE��Ŀ(double)��!!!������!!��
		vector<double> clusterSize(_RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum, 0);

		//��ʼ��clusterSize
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++)
			clusterSize[clusterIdx] = static_cast<double>(_RSU->getSystemPoint()->getGTTPoint()->m_ClusterVeUEIdList[clusterIdx].size());

		//���ȸ�������һ�����Ĵط���һ��TTI
		int basicNTTI = 0;
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			//����ô���������һ��VeUE��ֱ�ӷ����һ����λ��ʱ����Դ
			if (_RSU->getSystemPoint()->getGTTPoint()->m_ClusterVeUEIdList[clusterIdx].size() != 0) {
				get<2>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]) = 1;
				clusterSize[clusterIdx] -= VeUESizePerTTI;
				basicNTTI++;
			}
		}

		//���˸���Ϊ�յĴط����һ��TTI�⣬ʣ��ɷ����TTI����
		int remainNTTI = ns_RRM_TDM_DRA::gc_NTTI - basicNTTI;

		//����ʣ�µ���Դ�飬ѭ������һʱ϶�������ǰ������ߵĴأ�����֮�󣬸��Ķ�Ӧ�ı�������ȥ��ʱ϶��Ӧ��VeUE����
		while (remainNTTI > 0) {
			int dex = getMaxIndex(clusterSize);
			if (dex == -1) throw LTEV2X_Exception("������û�з����ʱ����Դ������ÿ�����ڵ�VeUE�Ѿ�Ϊ����");
			get<2>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[dex])++;
			remainNTTI--;
			clusterSize[dex] -= VeUESizePerTTI;
		}

		//��ʼ�������䷶Χ��������
		get<0>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[0]) = 0;
		get<1>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[0]) = get<0>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[0]) + get<2>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[0]) - 1;
		for (int clusterIdx = 1; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			get<0>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]) = get<1>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx - 1]) + 1;
			get<1>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]) = get<0>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]) + get<2>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]) - 1;
		}


		//����������д���RSU�ڵ�ÿһ������
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; ++clusterIdx) {
			for (int VeUEId : _RSU->getSystemPoint()->getGTTPoint()->m_ClusterVeUEIdList[clusterIdx])
				m_VeUEAry[VeUEId]->getTDM_DRAPoint()->m_ScheduleInterval = tuple<int, int>(get<0>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]), get<1>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]));
		}
	}
	writeClusterPerformInfo(g_FileClasterPerformInfo);
}


void RRM_TDM_DRA::uniformTDM(bool t_ClusterFlag) {
	if (!t_ClusterFlag)return;
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		//��ʼ��
		_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo = vector<tuple<int, int, int>>(_RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum, tuple<int, int, int>(0, 0, 0));

		int equalTimeLength = ns_RRM_TDM_DRA::gc_NTTI / _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum;

		int lastClusterLength = ns_RRM_TDM_DRA::gc_NTTI - equalTimeLength*_RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum + equalTimeLength;

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx] = tuple<int, int, int>(equalTimeLength*clusterIdx, equalTimeLength*(clusterIdx + 1) - 1, equalTimeLength);
		}

		//�޸����һ���ص�ʱ�򳤶ȣ���Ϊƽ���س����ܱ����������ˣ���ˣ�ƽ���س���*�������������ܵ���ʱ�䣬��˽������������һ����
		get<1>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[_RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum - 1]) = ns_RRM_TDM_DRA::gc_NTTI - 1;
		get<2>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[_RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum - 1]) = lastClusterLength;



		//����������д���RSU�ڵ�ÿһ������
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; ++clusterIdx) {
			for (int VeUEId : _RSU->getSystemPoint()->getGTTPoint()->m_ClusterVeUEIdList[clusterIdx])
				m_VeUEAry[VeUEId]->getTDM_DRAPoint()->m_ScheduleInterval = tuple<int, int>(get<0>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]), get<1>(_RSU->getTDM_DRAPoint()->m_ClusterTDRInfo[clusterIdx]));
		}
	}
	writeClusterPerformInfo(g_FileClasterPerformInfo);
}


void RRM_TDM_DRA::updateAccessEventIdList(bool t_ClusterFlag) {
	//���ȣ�����System������¼���������
	processEventList();
	//��Σ������ǰTTI������λ�ø��£���Ҫ������ȱ�
	if (t_ClusterFlag) {
		//����RSU����ĵ�������
		processScheduleInfoTableWhenLocationUpdate();

		//����RSU����ĵȴ�����
		processWaitEventIdListWhenLocationUpdate();

		//����System������л�����
		processSwitchListWhenLocationUpdate();
	}
	//����ɵȴ������ɽ����
	processWaitEventIdList();
}


void RRM_TDM_DRA::processEventList() {
	for (int eventId : getContext()->m_EventTTIList[getContext()->m_TTI]) {
		Event &event = getContext()->m_EventVec[eventId];
		int VeUEId = event.getVeUEId();
		int RSUId = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId;
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		int clusterIdx = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx;

		//�����¼�ѹ������¼��ȴ�����
		bool isEmergency = event.getMessageType() == EMERGENCY;
		_RSU->getTDM_DRAPoint()->pushToWaitEventIdList(isEmergency, clusterIdx, eventId);

		//������־
		getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, EVENT_TO_WAIT, -1, -1, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Trigger");
		writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, EVENT_TO_WAIT, eventId, -1, -1, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Trigger");
	}
}


void RRM_TDM_DRA::processScheduleInfoTableWhenLocationUpdate() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		//��ʼ���� m_ScheduleInfoTable
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {
				if (_RSU->getTDM_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx] == nullptr) continue;

				int eventId = _RSU->getTDM_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx]->eventId;
				int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();
				//��VeUE���ڵ�ǰRSU�У�Ӧ����ѹ��System������л�����
				if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId != _RSU->getSystemPoint()->getGTTPoint()->m_RSUId) {
					//ѹ��Switch����
					_RSU->getTDM_DRAPoint()->pushToSwitchEventIdList(m_SwitchEventIdList, eventId);

					//��ʣ�����bit����
					getContext()->m_EventVec[eventId].reset();

					//���ͷŸõ�����Ϣ����Դ
					Delete::safeDelete(_RSU->getTDM_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx]);

					//�ͷ�Pattern��Դ
					_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, SCHEDULETABLE_TO_SWITCH, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SCHEDULETABLE_TO_SWITCH, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "LocationUpdate");
				}
				else {
					//RSU�ڲ������˴��л�������ӵ��ȱ���ȡ����ѹ��ȴ�����
					if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx != clusterIdx) {
						//ѹ���RSU�ĵȴ�����
						bool isEmergency = getContext()->m_EventVec[eventId].getMessageType() == EMERGENCY;
						_RSU->getTDM_DRAPoint()->pushToWaitEventIdList(isEmergency, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, eventId);

						//���ͷŸõ�����Ϣ����Դ
						Delete::safeDelete(_RSU->getTDM_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx]);

						//�ͷ�Pattern��Դ
						_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

						//������־
						getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, SCHEDULETABLE_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
						writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SCHEDULETABLE_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
					}
				}
			}
		}
	}
}


void RRM_TDM_DRA::processWaitEventIdListWhenLocationUpdate() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		//��ʼ���� m_WaitEventIdList

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			/*  EMERGENCY  */
			list<int>::iterator it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first.begin();
			while (it != _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first.end()) {
				int eventId = *it;
				int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();
				//��VeUE�Ѿ����ڸ�RSU��Χ��
				if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId != _RSU->getSystemPoint()->getGTTPoint()->m_RSUId) {

					//������ӵ�System�����RSU�л�������
					_RSU->getTDM_DRAPoint()->pushToSwitchEventIdList(m_SwitchEventIdList, eventId);

					//����ӵȴ�������ɾ��
					it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first.erase(it);

					//��ʣ�����bit����
					getContext()->m_EventVec[eventId].reset();

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_SWITCH, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_SWITCH, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "LocationUpdate");
				}
				//��Ȼ���ڵ�ǰRSU��Χ�ڣ���λ�ڲ�ͬ�Ĵ�
				else if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx != clusterIdx) {

					//������ӵ����ڴصĵȴ�����
					_RSU->getTDM_DRAPoint()->pushToWaitEventIdList(true, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, eventId);

					//����ӵȴ������еĵ�ǰ��ɾ��
					it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first.erase(it);

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
				}
				else {
					it++;
					continue; //�������ڵ�ǰRSU�ĵȴ�����
				}
			}
			/*  EMERGENCY  */

			it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second.begin();
			while (it != _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second.end()) {
				int eventId = *it;
				int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();
				//��VeUE�Ѿ����ڸ�RSU��Χ��
				if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId != _RSU->getSystemPoint()->getGTTPoint()->m_RSUId) {

					//������ӵ�System�����RSU�л�������
					_RSU->getTDM_DRAPoint()->pushToSwitchEventIdList(m_SwitchEventIdList, eventId);

					//����ӵȴ�������ɾ��
					it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second.erase(it);

					//��ʣ�����bit����
					getContext()->m_EventVec[eventId].reset();

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_SWITCH, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_SWITCH, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "LocationUpdate");
				}
				//��Ȼ���ڵ�ǰRSU��Χ�ڣ���λ�ڲ�ͬ�Ĵ�
				else if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx != clusterIdx) {

					//������ӵ����ڴصĵȴ�����
					_RSU->getTDM_DRAPoint()->pushToWaitEventIdList(false, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, eventId);

					//����ӵȴ������еĵ�ǰ��ɾ��
					it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second.erase(it);

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
				}
				else {
					it++;
					continue; //�������ڵ�ǰRSU�ĵȴ�����
				}
			}
		}

	}
}


void RRM_TDM_DRA::processSwitchListWhenLocationUpdate() {
	list<int>::iterator it = m_SwitchEventIdList.begin();
	while (it != m_SwitchEventIdList.end()) {
		int eventId = *it;
		int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();
		int RSUId = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId;
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		int clusterIdx = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx;

		//ת��ȴ�����
		bool isEmergency = getContext()->m_EventVec[eventId].getMessageType() == EMERGENCY;
		_RSU->getTDM_DRAPoint()->pushToWaitEventIdList(isEmergency, clusterIdx, eventId);

		//��Switch���н���ɾ��
		it = m_SwitchEventIdList.erase(it);

		//������־
		getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, SWITCH_TO_WAIT, -1, -1, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "LocationUpdate");
		writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SWITCH_TO_WAIT, eventId, -1, -1, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "LocationUpdate");
	}
}


void RRM_TDM_DRA::processWaitEventIdList() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		/*  EMERGENCY  */
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			list<int>::iterator it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first.begin();
			while (it != _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first.end()) {
				int eventId = *it;
				//������¼�����Ҫ�˱ܣ���ת������
				if (getContext()->m_EventVec[eventId].tryAcccess()) {

					//ѹ���������
					_RSU->getTDM_DRAPoint()->pushToAccessEventIdList(true, clusterIdx, eventId);

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_ACCESS, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "CanAccess");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_ACCESS, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "CanAccess");

					//�����¼��ӵȴ�����ɾ��
					it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first.erase(it);
				}
				else {
					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WITHDRAWING, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "Withdraw");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WITHDRAWING, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "Withdraw");

					it++;
				}
			}
		}
		/*  EMERGENCY  */
		int clusterIdx = _RSU->getTDM_DRAPoint()->getClusterIdx(getContext()->m_TTI);
		list<int>::iterator it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second.begin();
		while (it != _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second.end()) {
			int eventId = *it;
			//������¼�����Ҫ�˱ܣ���ת������
			if (getContext()->m_EventVec[eventId].tryAcccess()) {

				//ѹ���������
				_RSU->getTDM_DRAPoint()->pushToAccessEventIdList(false, clusterIdx, eventId);

				//������־
				getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_ACCESS, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "CanAccess");
				writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_ACCESS, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "CanAccess");

				//�����¼��ӵȴ�����ɾ��
				it = _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second.erase(it);
			}
			else {
				//������־
				getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WITHDRAWING, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "Withdraw");
				writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WITHDRAWING, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "Withdraw");

				it++;
			}
		}
	}
}


void RRM_TDM_DRA::selectRBBasedOnP123() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		/*  EMERGENCY  */
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			vector<int> curAvaliableEmergencyPatternIdx;//��ǰ�ؿ��õ�EmergencyPattern���

			for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx++) {
				if (_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx]) {
					curAvaliableEmergencyPatternIdx.push_back(patternIdx);
				}
			}

			for (int eventId : _RSU->getTDM_DRAPoint()->m_AccessEventIdList[clusterIdx].first) {
				int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();

				//Ϊ��ǰ�û��ڿ��õ�EmergencyPattern�������ѡ��һ����ÿ���û��������ѡ�����EmergencyPattern��
				int patternIdx = m_VeUEAry[VeUEId]->getTDM_DRAPoint()->selectRBBasedOnP2(curAvaliableEmergencyPatternIdx);

				//�޶�ӦPattern���͵�pattern��Դ����
				if (patternIdx == -1) {
					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, ACCESS_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "AllBusy");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, ACCESS_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "AllBusy");

					continue;
				}
				//����Դ���Ϊռ��
				_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = false;

				//��������Ϣѹ��m_EmergencyTransimitEventIdList��
				_RSU->getTDM_DRAPoint()->pushToTransimitScheduleInfoList(new RRM_RSU::ScheduleInfo(eventId, VeUEId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, -1, patternIdx));

			}

		}
		/*  EMERGENCY  */

		//��ʼ�����Emergency���¼�
		int clusterIdx = _RSU->getTDM_DRAPoint()->getClusterIdx(getContext()->m_TTI);

		/*
		* ��ǰTTI���õ�Pattern����
		* �±�����Pattern����ı�ţ�����Emergency��һ���ǿյģ����Ǳ�����������Ų���ƫ��
		* ÿ���ڲ�vector���������Pattern���õ�Pattern���(����Pattern���)
		*/
		vector<vector<int>> curAvaliablePatternIdx(ns_RRM_TDM_DRA::gc_PatternTypeNum);

		for (int patternTypeIdx = 1; patternTypeIdx < ns_RRM_TDM_DRA::gc_PatternTypeNum; patternTypeIdx++) {
			for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[patternTypeIdx][0]; patternIdx <= ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[patternTypeIdx][1]; patternIdx++) {
				if (_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx]) {
					curAvaliablePatternIdx[patternTypeIdx].push_back(patternIdx);
				}
			}
		}

		for (int eventId : _RSU->getTDM_DRAPoint()->m_AccessEventIdList[clusterIdx].second) {
			int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();

			//Ϊ��ǰ�û��ڿ��õĶ�Ӧ���¼����͵�Pattern�������ѡ��һ����ÿ���û��������ѡ�����Pattern��
			MessageType messageType = getContext()->m_EventVec[eventId].getMessageType();
			int patternIdx = m_VeUEAry[VeUEId]->getTDM_DRAPoint()->selectRBBasedOnP2(curAvaliablePatternIdx[messageType]);

			//���û��������Ϣ����û��patternʣ����
			if (patternIdx == -1) {
				//������־
				getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, ACCESS_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "AllBusy");
				writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, ACCESS_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "AllBusy");

				continue;
			}

			//����Դ���Ϊռ��
			_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = false;

			//��������Ϣѹ��m_TransimitEventIdList��
			_RSU->getTDM_DRAPoint()->pushToTransimitScheduleInfoList(new RRM_RSU::ScheduleInfo(eventId, VeUEId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx));

		}
		//�����ȱ��е�ǰ���Լ���������û�ѹ�봫��������(���������ͷǽ���)
		_RSU->getTDM_DRAPoint()->pullFromScheduleInfoTable(getContext()->m_TTI);
	}
}


void RRM_TDM_DRA::delaystatistics() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		/*  EMERGENCY  */
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			//����ȴ�����
			for (int eventId : _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].first)
				getContext()->m_EventVec[eventId].increaseQueueDelay();
			//����˿����ڽ�Ҫ����Ĵ�������
			for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx++)
				for (RRM_RSU::ScheduleInfo* &p : _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx])
					getContext()->m_EventVec[p->eventId].increaseSendDelay();
		}
		/*  EMERGENCY  */


		//����ȴ�����
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int eventId : _RSU->getTDM_DRAPoint()->m_WaitEventIdList[clusterIdx].second)
				getContext()->m_EventVec[eventId].increaseQueueDelay();
		}

		//����˿����ڽ�Ҫ����Ĵ�������
		int clusterIdx = _RSU->getTDM_DRAPoint()->getClusterIdx(getContext()->m_TTI);
		for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {
			for (RRM_RSU::ScheduleInfo* &p : _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx])
				getContext()->m_EventVec[p->eventId].increaseSendDelay();
		}

		//����˿�λ�ڵ��ȱ��У��������ڵ�ǰ�ص��¼�
		for (int otherClusterIdx = 0; otherClusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; otherClusterIdx++) {
			if (otherClusterIdx == clusterIdx) continue;
			for (RRM_RSU::ScheduleInfo *p : _RSU->getTDM_DRAPoint()->m_ScheduleInfoTable[otherClusterIdx]) {
				if (p == nullptr) continue;
				getContext()->m_EventVec[p->eventId].increaseQueueDelay();
			}
		}
	}
}

void RRM_TDM_DRA::conflictListener() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		/*  EMERGENCY  */
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx++) {
				list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (lst.size() > 1) {//����һ��VeUE�ڵ�ǰTTI����Pattern�ϴ��䣬�������˳�ͻ��������ӵ��ȴ��б�
					for (RRM_RSU::ScheduleInfo* &info : lst) {
						//���Ƚ��¼�ѹ��ȴ��б�
						_RSU->getTDM_DRAPoint()->pushToWaitEventIdList(true, clusterIdx, info->eventId);

						//��ͻ������¼���״̬
						getContext()->m_EventVec[info->eventId].conflict();

						//������־
						getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, TRANSIMIT_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Conflict");
						writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, TRANSIMIT_TO_WAIT, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Conflict");

						//�ͷŵ�����Ϣ������ڴ���Դ
						Delete::safeDelete(info);
					}

					//�ͷ�Pattern��Դ
					_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

					lst.clear();
				}
			}
		}
		/*  EMERGENCY  */


		int clusterIdx = _RSU->getTDM_DRAPoint()->getClusterIdx(getContext()->m_TTI);
		for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {

			list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
			if (lst.size() > 1) {//����һ��VeUE�ڵ�ǰTTI����Pattern�ϴ��䣬�������˳�ͻ��������ӵ��ȴ��б�
				for (RRM_RSU::ScheduleInfo* &info : lst) {
					//���Ƚ��¼�ѹ��ȴ��б�
					_RSU->getTDM_DRAPoint()->pushToWaitEventIdList(false, clusterIdx, info->eventId);

					//��ͻ������¼���״̬
					getContext()->m_EventVec[info->eventId].conflict();

					//������־
					getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, TRANSIMIT_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Conflict");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, TRANSIMIT_TO_WAIT, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Conflict");

					//�ͷŵ�����Ϣ������ڴ���Դ
					Delete::safeDelete(info);
				}
				//�ͷ�Pattern��Դ
				_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

				lst.clear();
			}
		}
	}
}


void RRM_TDM_DRA::transimitPreparation() {
	//���������һ�θ�����Ϣ
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++)
		for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++)
			m_InterferenceVec[VeUEId][patternIdx].clear();

	//ͳ�Ʊ��εĸ�����Ϣ
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {
				list<RRM_RSU::ScheduleInfo*> &curList = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (curList.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
					RRM_RSU::ScheduleInfo *curInfo = *curList.begin();
					int curVeUEId = curInfo->VeUEId;
					for (int otherClusterIdx = 0; otherClusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; otherClusterIdx++) {
						if (otherClusterIdx == clusterIdx)continue;
						list<RRM_RSU::ScheduleInfo*> &otherList = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[otherClusterIdx][patternIdx];
						if (otherList.size() == 1) {//�������и�pattern���г����ڴ��䣬��ô���ó�����Ϊ���ų���
							RRM_RSU::ScheduleInfo *otherInfo = *otherList.begin();
							int otherVeUEId = otherInfo->VeUEId;
							m_InterferenceVec[curVeUEId][patternIdx].push_back(otherVeUEId);
						}
					}
				}
			}
		}
	}

	//����ÿ�����ĸ��ų����б�	
	for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {
		for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
			list<int>& interList = m_InterferenceVec[VeUEId][patternIdx];

			m_VeUEAry[VeUEId]->m_InterferenceVeUENum[patternIdx] = (int)interList.size();//д�������Ŀ

			m_VeUEAry[VeUEId]->m_InterferenceVeUEIdVec[patternIdx].assign(interList.begin(), interList.end());//д����ų���ID

			if (m_VeUEAry[VeUEId]->m_InterferenceVeUENum[patternIdx]>0) {
				g_FileTemp << "VeUEId: " << VeUEId << " [";
				for (auto c : m_VeUEAry[VeUEId]->m_InterferenceVeUEIdVec[patternIdx])
					g_FileTemp << c << ", ";
				g_FileTemp << " ]" << endl;
			}
		}
	}

	//����������˵�Ԫ���������Ӧ����
	long double start = clock();
	getContext()->m_GTTPoint->calculateInterference(m_InterferenceVec);
	long double end = clock();
	m_GTTTimeConsume += end - start;
}


void RRM_TDM_DRA::transimitStart() {
	long double start = clock();
	m_Threads.clear();
	for (int threadIdx = 0; threadIdx < m_ThreadNum; threadIdx++)
		m_Threads.push_back(thread(&RRM_TDM_DRA::transimitStartThread, &*this, m_ThreadsRSUIdRange[threadIdx].first, m_ThreadsRSUIdRange[threadIdx].second));
	for (int threadIdx = 0; threadIdx < m_ThreadNum; threadIdx++)
		m_Threads[threadIdx].join();
	long double end = clock();
	m_WTTimeConsume += end - start;
}

void RRM_TDM_DRA::transimitStartThread(int t_FromRSUId, int t_ToRSUId) {
	WT* copyWTPoint = getContext()->m_WTPoint->getCopy();//����ÿ���̵߳ĸ�ģ����в�ͬ��״̬���޷����������������ģ�����ڱ��μ���
	for (int RSUId = t_FromRSUId; RSUId <= t_ToRSUId; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		/*  EMERGENCY  */
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx++) {
				list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (lst.size() == 1) {
					RRM_RSU::ScheduleInfo *info = *lst.begin();
					int VeUEId = info->VeUEId;

					//����SINR����ȡ���Ʊ��뷽ʽ
					pair<int, int> subCarrierIdxRange = getOccupiedSubCarrierRange(getContext()->m_EventVec[info->eventId].getMessageType(), patternIdx);
					g_FileTemp << "Emergency PatternIdx = " << patternIdx << "  [" << subCarrierIdxRange.first << " , " << subCarrierIdxRange.second << " ]  " << endl;

					double factor = m_VeUEAry[VeUEId]->m_ModulationType * m_VeUEAry[VeUEId]->m_CodeRate;

					//�ñ��뷽ʽ�£���Pattern��һ��TTI���ɴ������Ч��Ϣbit����
					int maxEquivalentBitNum = (int)((double)(ns_RRM_TDM_DRA::gc_RBNumPerPatternType[EMERGENCY] * gc_BitNumPerRB)* factor);

					//����SINR
					double curSINR = 0;
					if (m_VeUEAry[VeUEId]->isNeedRecalculateSINR(patternIdx) || !m_VeUEAry[VeUEId]->isAlreadyCalculateSINR(patternIdx)) {//���Ʊ��뷽ʽ��Ҫ����ʱ
						curSINR = copyWTPoint->SINRCalculate(info->VeUEId, subCarrierIdxRange.first, subCarrierIdxRange.second, patternIdx);
						m_VeUEAry[VeUEId]->m_PreInterferenceVeUEIdVec[patternIdx] = m_VeUEAry[VeUEId]->m_InterferenceVeUEIdVec[patternIdx];
						m_VeUEAry[VeUEId]->m_PreSINR[patternIdx] = curSINR;
					}
					else
						curSINR = m_VeUEAry[VeUEId]->m_PreSINR[patternIdx];

					//��¼������Ϣ
					double tmpDistance = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_Distance[RSUId];
					if (curSINR < gc_CriticalPoint) {
						//��¼����
						getContext()->m_EventVec[info->eventId].packetLoss(tmpDistance);
					}
					info->transimitBitNum = maxEquivalentBitNum;
					info->currentPackageIdx = getContext()->m_EventVec[info->eventId].getCurrentPackageIdx();
					info->remainBitNum = getContext()->m_EventVec[info->eventId].getRemainBitNum();

					//�ñ��뷽ʽ�£���Pattern��һ��TTI�����ʵ�ʵ���Ч��Ϣbit��������������Ϣ״̬
					int realEquivalentBitNum = getContext()->m_EventVec[info->eventId].transimit(maxEquivalentBitNum, tmpDistance);

					//�ۼ�������
					getContext()->m_TTIRSUThroughput[getContext()->m_TTI][_RSU->getSystemPoint()->getGTTPoint()->m_RSUId] += realEquivalentBitNum;

					//������־
					getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, TRANSIMITTING, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Transimit");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, TRANSIMITTING, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Transimit");
				}
			}
		}
		/*  EMERGENCY  */


		int clusterIdx = _RSU->getTDM_DRAPoint()->getClusterIdx(getContext()->m_TTI);
		for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {

			list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
			if (lst.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
				RRM_RSU::ScheduleInfo *info = *lst.begin();
				int VeUEId = info->VeUEId;

				int patternType = getPatternType(patternIdx);

				//����SINR����ȡ���Ʊ��뷽ʽ
				pair<int, int> subCarrierIdxRange = getOccupiedSubCarrierRange(getContext()->m_EventVec[info->eventId].getMessageType(), patternIdx);
				g_FileTemp << "NonEmergencyPatternIdx = " << patternIdx << "  [" << subCarrierIdxRange.first << " , " << subCarrierIdxRange.second << " ]  " << ((patternType == 0) ? "Emergency" : (patternType == 1 ? "Period" : "Data")) << endl;

				double factor = m_VeUEAry[VeUEId]->m_ModulationType * m_VeUEAry[VeUEId]->m_CodeRate;

				//�ñ��뷽ʽ�£���Pattern��һ��TTI���ɴ������Ч��Ϣbit����
				int maxEquivalentBitNum = (int)((double)(ns_RRM_TDM_DRA::gc_RBNumPerPatternType[patternType] * gc_BitNumPerRB)* factor);

				//����SINR
				double curSINR = 0;
				if (m_VeUEAry[VeUEId]->isNeedRecalculateSINR(patternIdx) || !m_VeUEAry[VeUEId]->isAlreadyCalculateSINR(patternIdx)) {//���Ʊ��뷽ʽ��Ҫ����ʱ
					curSINR = copyWTPoint->SINRCalculate(info->VeUEId, subCarrierIdxRange.first, subCarrierIdxRange.second, patternIdx);
					m_VeUEAry[VeUEId]->m_PreInterferenceVeUEIdVec[patternIdx] = m_VeUEAry[VeUEId]->m_InterferenceVeUEIdVec[patternIdx];
					m_VeUEAry[VeUEId]->m_PreSINR[patternIdx] = curSINR;
				}
				else
					curSINR = m_VeUEAry[VeUEId]->m_PreSINR[patternIdx];

				//��¼������Ϣ
				double tmpDistance = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_Distance[RSUId];
				if (curSINR < gc_CriticalPoint) {
					//��¼����
					getContext()->m_EventVec[info->eventId].packetLoss(tmpDistance);
				}
				info->transimitBitNum = maxEquivalentBitNum;
				info->currentPackageIdx = getContext()->m_EventVec[info->eventId].getCurrentPackageIdx();
				info->remainBitNum = getContext()->m_EventVec[info->eventId].getRemainBitNum();

				//�ñ��뷽ʽ�£���Pattern��һ��TTI�����ʵ�ʵ���Ч��Ϣbit��������������Ϣ״̬
				int realEquivalentBitNum = getContext()->m_EventVec[info->eventId].transimit(maxEquivalentBitNum, tmpDistance);

				//�ۼ�������
				getContext()->m_TTIRSUThroughput[getContext()->m_TTI][_RSU->getSystemPoint()->getGTTPoint()->m_RSUId] += realEquivalentBitNum;

				//������־
				getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, TRANSIMITTING, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Transimit");
				writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, TRANSIMITTING, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Transimit");
			}
		}
	}
	copyWTPoint->freeCopy();//getCopy��ͨ��new�����ģ���������ͷ���Դ
}

void RRM_TDM_DRA::writeScheduleInfo(ofstream& t_File) {
	if (!getContext()->m_Config.scheduleLogIsOn)return;
	t_File << "[ TTI = " << left << setw(3) << getContext()->m_TTI << "]" << endl;
	t_File << "{" << endl;
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		int NonEmergencyClusterIdx = _RSU->getTDM_DRAPoint()->getClusterIdx(getContext()->m_TTI);

		t_File << "    RSU[" << _RSU->getSystemPoint()->getGTTPoint()->m_RSUId << "][TTI = " << getContext()->m_TTI << "]" << endl;
		t_File << "    {" << endl;
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			t_File << "        Cluster[" << clusterIdx << "]" << endl;
			t_File << "        {" << endl;

			t_File << "            EMERGENCY:" << endl;
			for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[EMERGENCY][0]; patternIdx <= ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[EMERGENCY][1]; patternIdx++) {
				bool isAvaliable = _RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx];
				t_File << "                Pattern[" << left << setw(3) << patternIdx << "] : " << (isAvaliable ? "Available" : "Unavailable") << endl;
				if (!isAvaliable) {
					RRM_RSU::ScheduleInfo *info = *(_RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx].begin());
					t_File << info->toScheduleString(5) << endl;
				}
			}
			if (clusterIdx != NonEmergencyClusterIdx)continue;
			t_File << "            PERIOD:" << endl;
			for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[PERIOD][0]; patternIdx <= ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[PERIOD][1]; patternIdx++) {
				bool isAvaliable = _RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx];
				t_File << "                Pattern[" << left << setw(3) << patternIdx << "] : " << (isAvaliable ? "Available" : "Unavailable") << endl;
				if (!isAvaliable) {
					RRM_RSU::ScheduleInfo *info = *(_RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx].begin());
					t_File << info->toScheduleString(5) << endl;
				}
			}
			t_File << "            DATA:" << endl;
			for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[DATA][0]; patternIdx <= ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[DATA][1]; patternIdx++) {
				bool isAvaliable = _RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx];
				t_File << "                Pattern[" << left << setw(3) << patternIdx << "] : " << (isAvaliable ? "Available" : "Unavailable") << endl;
				if (!isAvaliable) {
					RRM_RSU::ScheduleInfo *info = *(_RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx].begin());
					t_File << info->toScheduleString(5) << endl;
				}
			}
			t_File << "        }" << endl;
		}
		t_File << "{" << endl;
	}
	t_File << "}" << endl;
	t_File << "\n\n" << endl;
}

void RRM_TDM_DRA::transimitEnd() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		/*  EMERGENCY  */
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx++) {
				list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (lst.size() == 1) {
					RRM_RSU::ScheduleInfo* &info = *lst.begin();
					//�Ѿ�������ϣ�����Դ�ͷ�
					if (getContext()->m_EventVec[info->eventId].isFinished()) {

						//������־
						getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, SUCCEED, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Succeed");
						writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SUCCEED, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Succeed");

						//�ͷŵ�����Ϣ������ڴ���Դ
						Delete::safeDelete(info);

						//�ͷ�Pattern��Դ
						_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;
					}
					else {//��δ�������
						_RSU->getTDM_DRAPoint()->pushToScheduleInfoTable(info);
						info = nullptr;
					}
				}
				lst.clear();
			}
		}
		/*  EMERGENCY  */


		int clusterIdx = _RSU->getTDM_DRAPoint()->getClusterIdx(getContext()->m_TTI);
		for (int patternIdx = ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[EMERGENCY]; patternIdx < ns_RRM_TDM_DRA::gc_TotalPatternNum; patternIdx++) {

			list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getTDM_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
			if (lst.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
				RRM_RSU::ScheduleInfo* &info = *lst.begin();
				//˵���������Ѿ��������
				if (getContext()->m_EventVec[info->eventId].isFinished()) {

					//������־								
					getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, SUCCEED, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Succeed");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SUCCEED, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Succeed");

					//�ͷŵ�����Ϣ������ڴ���Դ
					Delete::safeDelete(info);

					//�ͷ�Pattern��Դ
					_RSU->getTDM_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;
				}
				else {//��������δ���꣬����ѹ��m_ScheduleInfoTable
					_RSU->getTDM_DRAPoint()->pushToScheduleInfoTable(info);
					info = nullptr;
				}
			}
			//������󣬽���pattern�ϵ�������գ���ʱҪ��������ǿգ�Ҫ������nullptrָ�룩
			lst.clear();
		}
	}
}


void RRM_TDM_DRA::writeTTILogInfo(ofstream& t_File, int t_TTI, EventLogType t_EventLogType, int t_EventId, int t_FromRSUId, int t_FromClusterIdx, int t_FromPatternIdx, int t_ToRSUId, int t_ToClusterIdx, int t_ToPatternIdx, std::string t_Description) {
	if (!getContext()->m_Config.scheduleLogIsOn)return;
	stringstream ss;
	switch (t_EventLogType) {
	case TRANSIMITTING:
		ss << " - Transimiting  At: RSU[" << t_FromRSUId << "] - ClusterIdx[" << t_FromClusterIdx << "] - PatternIdx[" << t_FromPatternIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case WITHDRAWING:
		ss << " - Withdrawing  At: RSU[" << t_FromRSUId << "] - ClusterIdx[" << t_FromClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case SUCCEED:
		ss << " - Transimit Succeed At: RSU[" << t_FromRSUId << "] - ClusterIdx[" << t_FromClusterIdx << "] - PatternIdx[" << t_FromPatternIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case EVENT_TO_WAIT:
		ss << " - From: EventList - To: RSU[" << t_ToRSUId << "]'s WaitEventIdList[" << t_ToClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case SCHEDULETABLE_TO_SWITCH:
		ss << " - From: RSU[" << t_FromRSUId << "]'s ScheduleTable[" << t_FromClusterIdx << "][" << t_FromPatternIdx << "] - To: SwitchList";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case SCHEDULETABLE_TO_WAIT:
		ss << " - From: RSU[" << t_FromRSUId << "]'s ScheduleTable[" << t_FromClusterIdx << "][" << t_FromPatternIdx << "] - To: RSU[" << t_ToRSUId << "]'s WaitEventIdList[" << t_ToClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case WAIT_TO_SWITCH:
		ss << " - From: RSU[" << t_FromRSUId << "]'s WaitEventIdList[" << t_FromClusterIdx << "] - To: SwitchList";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case WAIT_TO_WAIT:
		ss << " - From: RSU[" << t_FromRSUId << "]'s WaitEventIdList[" << t_FromClusterIdx << "] - To: RSU[" << t_ToRSUId << "]'s WaitEventIdList[" << t_ToClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case WAIT_TO_ACCESS:
		ss << " - From: RSU[" << t_FromRSUId << "]'s WaitEventIdList[" << t_FromClusterIdx << "] - To: RSU[" << t_ToRSUId << "]'s AccessEventIdList[" << t_ToClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case ACCESS_TO_WAIT:
		ss << " - From: RSU[" << t_FromRSUId << "]'s AccessEventIdList[" << t_FromClusterIdx << "] - To: RSU[" << t_ToRSUId << "]'s WaitEventIdList[" << t_ToClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case SWITCH_TO_WAIT:
		ss << " - From: SwitchList - To: RSU[" << t_ToRSUId << "]'s WaitEventIdList[" << t_ToClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	case TRANSIMIT_TO_WAIT:
		ss << " - From: RSU[" << t_FromRSUId << "]'s TransimitScheduleInfoList[" << t_FromClusterIdx << "][" << t_FromPatternIdx << "] - To: RSU[" << t_ToRSUId << "]'s WaitEventIdList[" << t_ToClusterIdx << "]";
		t_File << "{ TTI : " << left << setw(3) << t_TTI << " - EventId = " << left << setw(3) << t_EventId << " - Description : <" << left << setw(10) << t_Description + ">" << ss.str() << " }" << endl;
		break;
	}
}


void RRM_TDM_DRA::writeClusterPerformInfo(ofstream &t_File) {
	t_File << "[ TTI = " << left << setw(3) << getContext()->m_TTI << "]" << endl;
	t_File << "{" << endl;

	//��ӡVeUE��Ϣ
	t_File << "    VUE Info: " << endl;
	t_File << "    {" << endl;
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		RRM_VeUE &_VeUE = *(m_VeUEAry[VeUEId]);
		t_File << _VeUE.getTDM_DRAPoint()->toString(2) << endl;
	}
	t_File << "    }\n" << endl;

	////��ӡ��վ��Ϣ
	//out << "    eNB Info: " << endl;
	//out << "    {" << endl;
	//for (int eNBId = 0; eNBId < m_Config.eNBNum; eNBId++) {
	//	ceNB &_eNB = m_eNBAry[eNBId];
	//	out << _eNB.toString(2) << endl;
	//}
	//out << "    }\n" << endl;

	//��ӡRSU��Ϣ
	t_File << "    RSU Info: " << endl;
	t_File << "    {" << endl;
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		t_File << _RSU->getTDM_DRAPoint()->toString(2) << endl;
	}
	t_File << "    }" << endl;

	t_File << "}\n\n";
}


int RRM_TDM_DRA::getMaxIndex(const vector<double>&t_ClusterSize) {
	double max = 0;
	int dex = -1;
	for (int i = 0; i < static_cast<int>(t_ClusterSize.size()); i++) {
		if (t_ClusterSize[i] > max) {
			dex = i;
			max = t_ClusterSize[i];
		}
	}
	return dex;
}


int RRM_TDM_DRA::getPatternType(int t_PatternIdx) {
	//patternIdxָ�����¼����͵�Pattern�ľ�����ţ���0��ʼ��ţ�����Emergency
	for (int patternType = 0; patternType < ns_RRM_TDM_DRA::gc_PatternTypeNum; patternType++) {
		if (t_PatternIdx >= ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[patternType][0] && t_PatternIdx <= ns_RRM_TDM_DRA::gc_PatternTypePatternIdxInterval[patternType][1])
			return patternType;
	}
	throw LTEV2X_Exception("getPatternType");
}


pair<int, int> RRM_TDM_DRA::getOccupiedSubCarrierRange(MessageType t_MessageType, int t_PatternIdx) {

	pair<int, int> res;

	int offset = 0;
	for (int i = 0; i < t_MessageType; i++) {
		offset += ns_RRM_TDM_DRA::gc_RBNumPerPatternType[i] * ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[i];
		t_PatternIdx -= ns_RRM_TDM_DRA::gc_PatternNumPerPatternType[i];
	}
	res.first = offset + ns_RRM_TDM_DRA::gc_RBNumPerPatternType[t_MessageType] * t_PatternIdx;
	res.second = offset + ns_RRM_TDM_DRA::gc_RBNumPerPatternType[t_MessageType] * (t_PatternIdx + 1) - 1;

	res.first *= 12;
	res.second = (res.second + 1) * 12 - 1;
	return res;
}
