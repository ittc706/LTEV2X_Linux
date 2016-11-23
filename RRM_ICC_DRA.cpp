/*
* =====================================================================================
*
*       Filename:  RRM_ICC_DRA.cpp
*
*    Description:  RRM_ICC_DRAģ��
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
#include<sstream>
#include"RRM_ICC_DRA.h"
#include"Function.h"
#include"System.h"

using namespace std;

default_random_engine RRM_ICC_DRA_VeUE::s_Engine((unsigned)time(NULL));


RRM_ICC_DRA_VeUE::RRM_ICC_DRA_VeUE() :RRM_VeUE(ns_RRM_ICC_DRA::gc_TotalPatternNum) {}


std::string RRM_ICC_DRA_VeUE::toString(int t_NumTab) {
	string indent;
	for (int i = 0; i < t_NumTab; i++)
		indent.append("    ");

	ostringstream ss;
	ss << indent << "{ VeUEId = " << left << setw(3) << getSystemPoint()->getGTTPoint()->m_VeUEId;
	ss << " , RSUId = " << left << setw(3) << getSystemPoint()->getGTTPoint()->m_RSUId;
	ss << " , ClusterIdx = " << left << setw(3) << getSystemPoint()->getGTTPoint()->m_ClusterIdx << " }";
	return ss.str();
}


RRM_ICC_DRA_RSU::RRM_ICC_DRA_RSU() {}


void RRM_ICC_DRA_RSU::initialize() {
	m_AccessEventIdList = vector<list<int>>(getSystemPoint()->getGTTPoint()->m_ClusterNum);
	m_WaitEventIdList = vector<list<int>>(getSystemPoint()->getGTTPoint()->m_ClusterNum);
	m_PatternIsAvailable = vector<vector<bool>>(getSystemPoint()->getGTTPoint()->m_ClusterNum, vector<bool>(ns_RRM_ICC_DRA::gc_TotalPatternNum, true));
	m_ScheduleInfoTable = vector<vector<ScheduleInfo*>>(getSystemPoint()->getGTTPoint()->m_ClusterNum, vector<ScheduleInfo*>(ns_RRM_ICC_DRA::gc_TotalPatternNum, nullptr));
	m_TransimitScheduleInfoList = vector<vector<list<ScheduleInfo*>>>(getSystemPoint()->getGTTPoint()->m_ClusterNum, vector<list<ScheduleInfo*>>(ns_RRM_ICC_DRA::gc_TotalPatternNum));
}


string RRM_ICC_DRA_RSU::toString(int t_NumTab) {
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


	//������Ϣ
	ss << indent << "}" << endl;
	return ss.str();
}



RRM_ICC_DRA::RRM_ICC_DRA(System* t_Context) :
	RRM(t_Context) {
	m_ThreadNum = t_Context->m_Config.threadNum;

	m_InterferenceVec = vector<vector<list<int>>>(getContext()->m_Config.VeUENum, vector<list<int>>(ns_RRM_ICC_DRA::gc_TotalPatternNum));
	m_ThreadsRSUIdRange = vector<pair<int, int>>(m_ThreadNum);

	int num = getContext()->m_Config.RSUNum / m_ThreadNum;
	for (int threadIdx = 0; threadIdx < m_ThreadNum; threadIdx++) {
		m_ThreadsRSUIdRange[threadIdx] = pair<int, int>(threadIdx*num, (threadIdx + 1)*num - 1);
	}
	m_ThreadsRSUIdRange[m_ThreadNum - 1].second = getContext()->m_Config.RSUNum - 1;//�������һ���߽�
}


void RRM_ICC_DRA::initialize() {
	//��ʼ��VeUE�ĸ�ģ���������		 
	m_VeUEAry = new RRM_VeUE*[getContext()->m_Config.VeUENum];
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		m_VeUEAry[VeUEId] = new RRM_ICC_DRA_VeUE();
	}

	//��ʼ��RSU�ĸ�ģ���������
	m_RSUAry = new RRM_RSU*[getContext()->m_Config.RSUNum];
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		m_RSUAry[RSUId] = new RRM_ICC_DRA_RSU();
	}
}


void RRM_ICC_DRA::cleanWhenLocationUpdate() {
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		for (vector<int>& preInterferenceVeUEIdVec : m_VeUEAry[VeUEId]->m_PreInterferenceVeUEIdVec)
			preInterferenceVeUEIdVec.clear();

		m_VeUEAry[VeUEId]->m_PreSINR.assign(ns_RRM_ICC_DRA::gc_TotalPatternNum, (numeric_limits<double>::min)());
	}
}


void RRM_ICC_DRA::schedule() {
	bool isLocationUpdate = getContext()->m_TTI  % getContext()->m_Config.locationUpdateNTTI == 0;

	//д�����λ����Ϣ
	writeClusterPerformInfo(isLocationUpdate, g_FileClasterPerformInfo);

	//��Դ������Ϣ���:����ÿ��RSU�ڵĽ��������
	informationClean();

	//���µȴ�����
	updateAccessEventIdList(isLocationUpdate);

	//��Դѡ��
	selectRBBasedOnP123();

	//ͳ��ʱ����Ϣ
	delaystatistics();

	//֡����ͻ
	conflictListener();

	//����������˵�Ԫ���������Ӧ����
	transimitPreparation();

	//���俪ʼ
	transimitStart();

	//д�������Ϣ
	writeScheduleInfo(g_FileScheduleInfo);

	//�������
	transimitEnd();
}


void RRM_ICC_DRA::informationClean() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			_RSU->getICC_DRAPoint()->m_AccessEventIdList[clusterIdx].clear();
		}
	}
}


void RRM_ICC_DRA::updateAccessEventIdList(bool t_ClusterFlag) {
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


void RRM_ICC_DRA::processEventList() {
	for (int eventId : getContext()->m_EventTTIList[getContext()->m_TTI]) {
		Event &event = getContext()->m_EventVec[eventId];
		int VeUEId = event.getVeUEId();
		int RSUId = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId;
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		int clusterIdx = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx;

		//�����¼�ѹ��ȴ�����
		_RSU->getICC_DRAPoint()->pushToWaitEventIdList(clusterIdx, eventId);

		//������־
		getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, EVENT_TO_WAIT, -1, -1, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Trigger");
		writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, EVENT_TO_WAIT, eventId, -1, -1, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Trigger");
	}
}


void RRM_ICC_DRA::processScheduleInfoTableWhenLocationUpdate() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {
				if (_RSU->getICC_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx] == nullptr) continue;

				int eventId = _RSU->getICC_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx]->eventId;
				int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();
				//��VeUE���ڵ�ǰRSU�У�Ӧ����ѹ��System������л�����
				if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId != _RSU->getSystemPoint()->getGTTPoint()->m_RSUId) {
					//ѹ��Switch����
					_RSU->getICC_DRAPoint()->pushToSwitchEventIdList(m_SwitchEventIdList, eventId);

					//��ʣ�����bit����
					getContext()->m_EventVec[eventId].reset();

					//���ͷŸõ�����Ϣ����Դ
					Delete::safeDelete(_RSU->getICC_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx]);

					//�ͷ�Pattern��Դ
					_RSU->getICC_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, SCHEDULETABLE_TO_SWITCH, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SCHEDULETABLE_TO_SWITCH, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "LocationUpdate");
				}
				else {
					//RSU�ڲ������˴��л�������ӵ��ȱ���ȡ����ѹ��ȴ�����
					if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx != clusterIdx) {
						//ѹ���RSU�ĵȴ�����
						_RSU->getICC_DRAPoint()->pushToWaitEventIdList(m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, eventId);

						//���ͷŸõ�����Ϣ����Դ
						Delete::safeDelete(_RSU->getICC_DRAPoint()->m_ScheduleInfoTable[clusterIdx][patternIdx]);

						//�ͷ�Pattern��Դ
						_RSU->getICC_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

						//������־
						getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, SCHEDULETABLE_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "LocationUpdate");
						writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SCHEDULETABLE_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "LocationUpdate");
					}
				}
			}
		}
	}
}


void RRM_ICC_DRA::processWaitEventIdListWhenLocationUpdate() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			//��ʼ���� m_WaitEventIdList
			list<int>::iterator it = _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx].begin();
			while (it != _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx].end()) {
				int eventId = *it;
				int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();
				//��VeUE�Ѿ����ڸ�RSU��Χ��
				if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId != _RSU->getSystemPoint()->getGTTPoint()->m_RSUId) {
					//������ӵ�System�����RSU�л�������
					_RSU->getICC_DRAPoint()->pushToSwitchEventIdList(m_SwitchEventIdList, eventId);

					//����ӵȴ�������ɾ��
					it = _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx].erase(it);

					//��ʣ�����bit����
					getContext()->m_EventVec[eventId].reset();

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_SWITCH, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_SWITCH, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, -1, -1, -1, "LocationUpdate");
				}
				//��Ȼ���ڵ�ǰRSU��Χ�ڣ���λ�ڲ�ͬ�Ĵ�
				else if (m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx != clusterIdx) {

					//������ӵ����ڴصĵȴ�����
					_RSU->getICC_DRAPoint()->pushToWaitEventIdList(m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, eventId);

					//����ӵȴ������еĵ�ǰ��ɾ��
					it = _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx].erase(it);

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx, -1, "LocationUpdate");
				}
				//��Ȼ���ڵ�ǰRSU��Χ��
				else {
					it++;
					continue; //�������ڵ�ǰRSU�ĵȴ�����
				}
			}
		}
	}
}


void RRM_ICC_DRA::processSwitchListWhenLocationUpdate() {
	list<int>::iterator it = m_SwitchEventIdList.begin();
	while (it != m_SwitchEventIdList.end()) {
		int eventId = *it;
		int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();
		int RSUId = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_RSUId;
		RRM_RSU *_RSU = m_RSUAry[RSUId];
		int clusterIdx = m_VeUEAry[VeUEId]->getSystemPoint()->getGTTPoint()->m_ClusterIdx;

		//ת��ȴ�����
		_RSU->getICC_DRAPoint()->pushToWaitEventIdList(clusterIdx, eventId);

		//��Switch���н���ɾ��
		it = m_SwitchEventIdList.erase(it);

		//������־
		getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, SWITCH_TO_WAIT, -1, -1, -1, RSUId, clusterIdx, -1, "LocationUpdate");
		writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SWITCH_TO_WAIT, eventId, -1, -1, -1, RSUId, clusterIdx, -1, "LocationUpdate");
	}
}


void RRM_ICC_DRA::processWaitEventIdList() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			list<int>::iterator it = _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx].begin();
			while (it != _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx].end()) {
				int eventId = *it;
				//������¼�����Ҫ�˱ܣ���ת������
				if (getContext()->m_EventVec[eventId].tryAcccess()) {

					//ѹ���������
					_RSU->getICC_DRAPoint()->pushToAccessEventIdList(clusterIdx, eventId);

					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, WAIT_TO_ACCESS, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "CanAccess");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, WAIT_TO_ACCESS, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "CanAccess");

					//�����¼��ӵȴ�����ɾ��
					it = _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx].erase(it);
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
}


void RRM_ICC_DRA::selectRBBasedOnP123() {
	for (int RSUId = 0; RSUId <getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {

			/*
			* ��ǰTTI��ǰ�ؿ��õ�Pattern����
			* ÿ���ڲ�vector���������Pattern���õ�Pattern���(����Pattern���)
			*/
			vector<int> curAvaliablePatternIdx;

			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {
				if (_RSU->getICC_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx]) {
					curAvaliablePatternIdx.push_back(patternIdx);
				}
			}

			for (int eventId : _RSU->getICC_DRAPoint()->m_AccessEventIdList[clusterIdx]) {//�����ô��ڽ��������е��¼�

				int VeUEId = getContext()->m_EventVec[eventId].getVeUEId();

				//Ϊ��ǰ�û��ڿ��õĶ�Ӧ���¼����͵�Pattern�������ѡ��һ����ÿ���û��������ѡ�����Pattern��
				int patternIdx = m_VeUEAry[VeUEId]->getICC_DRAPoint()->selectRBBasedOnP2(curAvaliablePatternIdx);

				//���û��������Ϣ����û��patternʣ����
				if (patternIdx == -1) {
					//������־
					getContext()->m_EventVec[eventId].addEventLog(getContext()->m_TTI, ACCESS_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "AllBusy");
					writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, ACCESS_TO_WAIT, eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "AllBusy");

					continue;
				}

				//����Դ���Ϊռ��
				_RSU->getICC_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = false;

				//��������Ϣѹ��m_TransimitEventIdList��
				_RSU->getICC_DRAPoint()->pushToTransmitScheduleInfoList(new RRM_RSU::ScheduleInfo(eventId, VeUEId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx));

			}
		}
		//�����ȱ��е�ǰ���Լ���������û�ѹ�봫��������
		_RSU->getICC_DRAPoint()->pullFromScheduleInfoTable(getContext()->m_TTI);
	}
}


void RRM_ICC_DRA::delaystatistics() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];


		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			//����ȴ�����
			for (int eventId : _RSU->getICC_DRAPoint()->m_WaitEventIdList[clusterIdx])
				getContext()->m_EventVec[eventId].increaseQueueDelay();

			//����˿����ڽ�Ҫ����Ĵ�������
			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {
				for (RRM_RSU::ScheduleInfo* &p : _RSU->getICC_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx])
					getContext()->m_EventVec[p->eventId].increaseSendDelay();
			}
		}
	}
}


void RRM_ICC_DRA::conflictListener() {
	for (int RSUId = 0; RSUId <getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {

				list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getICC_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (lst.size() > 1) {//����һ��VeUE�ڵ�ǰTTI����Pattern�ϴ��䣬�������˳�ͻ��������ӵ��ȴ��б�
					for (RRM_RSU::ScheduleInfo* &info : lst) {
						//���Ƚ��¼�ѹ��ȴ��б�
						_RSU->getICC_DRAPoint()->pushToWaitEventIdList(clusterIdx, info->eventId);

						//��ͻ������¼���״̬
						getContext()->m_EventVec[info->eventId].conflict();

						//������־
						getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, TRANSIMIT_TO_WAIT, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Conflict");
						writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, TRANSIMIT_TO_WAIT, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, -1, "Conflict");

						//�ͷŵ�����Ϣ������ڴ���Դ
						Delete::safeDelete(info);
					}
					//�ͷ�Pattern��Դ
					_RSU->getICC_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

					lst.clear();
				}
			}
		}
	}
}


void RRM_ICC_DRA::transimitPreparation() {
	//���������һ�θ�����Ϣ
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++)
		for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++)
			m_InterferenceVec[VeUEId][patternIdx].clear();

	//ͳ�Ʊ��εĸ�����Ϣ
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {
				list<RRM_RSU::ScheduleInfo*> &curList = _RSU->getICC_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (curList.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
					RRM_RSU::ScheduleInfo *curInfo = *curList.begin();
					int curVeUEId = curInfo->VeUEId;
					for (int otherClusterIdx = 0; otherClusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; otherClusterIdx++) {
						if (otherClusterIdx == clusterIdx)continue;
						list<RRM_RSU::ScheduleInfo*> &otherList = _RSU->getICC_DRAPoint()->m_TransimitScheduleInfoList[otherClusterIdx][patternIdx];
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
	for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {
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


void RRM_ICC_DRA::transimitStart() {
	long double start = clock();
	m_Threads.clear();
	for (int threadIdx = 0; threadIdx < m_ThreadNum; threadIdx++)
		m_Threads.push_back(thread(&RRM_ICC_DRA::transimitStartThread, &*this, m_ThreadsRSUIdRange[threadIdx].first, m_ThreadsRSUIdRange[threadIdx].second));
	for (int threadIdx = 0; threadIdx < m_ThreadNum; threadIdx++)
		m_Threads[threadIdx].join();
	long double end = clock();
	m_WTTimeConsume += end - start;
}


void RRM_ICC_DRA::transimitStartThread(int t_FromRSUId, int t_ToRSUId) {
	WT* copyWTPoint = getContext()->m_WTPoint->getCopy();//����ÿ���̵߳ĸ�ģ����в�ͬ��״̬���޷����������������ģ�����ڱ��μ���
	for (int RSUId = t_FromRSUId; RSUId <= t_ToRSUId; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {

				list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getICC_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (lst.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
					RRM_RSU::ScheduleInfo *info = *lst.begin();
					int VeUEId = info->VeUEId;

					//����SINR����ȡ���Ʊ��뷽ʽ
					pair<int, int> subCarrierIdxRange = getOccupiedSubCarrierRange(patternIdx);
					g_FileTemp << "NonEmergencyPatternIdx = " << patternIdx << "  [" << subCarrierIdxRange.first << " , " << subCarrierIdxRange.second << " ]  " << endl;

					double factor = m_VeUEAry[VeUEId]->m_ModulationType * m_VeUEAry[VeUEId]->m_CodeRate;

					//�ñ��뷽ʽ�£���Pattern��һ��TTI���ɴ������Ч��Ϣbit����
					int maxEquivalentBitNum = (int)((double)(ns_RRM_ICC_DRA::gc_RBNumPerPattern * gc_BitNumPerRB)* factor);

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
	}
	copyWTPoint->freeCopy();//getCopy��ͨ��new�����ģ���������ͷ���Դ
}


void RRM_ICC_DRA::writeScheduleInfo(ofstream& t_File) {
	if (!getContext()->m_Config.scheduleLogIsOn)return;
	t_File << "[ TTI = " << left << setw(3) << getContext()->m_TTI << "]" << endl;
	t_File << "{" << endl;
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {

		RRM_RSU *_RSU = m_RSUAry[RSUId];
		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			t_File << "    RSU[" << _RSU->getSystemPoint()->getGTTPoint()->m_RSUId << "] :" << endl;
			t_File << "    {" << endl;
			t_File << "        Cluster[" << clusterIdx << "] :" << endl;
			t_File << "        {" << endl;
			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {
				t_File << "            Pattern[ " << left << setw(3) << patternIdx << "] : " << endl;
				bool isAvaliable = _RSU->getICC_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx];
				if (!isAvaliable) {
					RRM_RSU::ScheduleInfo* &info = *(_RSU->getICC_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx].begin());
					if (info == nullptr) throw Exp("logic error");
					t_File << info->toScheduleString(3) << endl;
				}
			}
			t_File << "        }" << endl;
			t_File << "    }" << endl;
		}

	}
	t_File << "}" << endl;
	t_File << "\n\n" << endl;
}


void RRM_ICC_DRA::transimitEnd() {
	for (int RSUId = 0; RSUId < getContext()->m_Config.RSUNum; RSUId++) {
		RRM_RSU *_RSU = m_RSUAry[RSUId];

		for (int clusterIdx = 0; clusterIdx < _RSU->getSystemPoint()->getGTTPoint()->m_ClusterNum; clusterIdx++) {
			for (int patternIdx = 0; patternIdx < ns_RRM_ICC_DRA::gc_TotalPatternNum; patternIdx++) {

				list<RRM_RSU::ScheduleInfo*> &lst = _RSU->getICC_DRAPoint()->m_TransimitScheduleInfoList[clusterIdx][patternIdx];
				if (lst.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
					RRM_RSU::ScheduleInfo* &info = *lst.begin();
					//˵���������Ѿ��������
					if (getContext()->m_EventVec[info->eventId].isFinished()) {

						//������־
						getContext()->m_EventVec[info->eventId].addEventLog(getContext()->m_TTI, SUCCEED, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Succeed");
						writeTTILogInfo(g_FileTTILogInfo, getContext()->m_TTI, SUCCEED, info->eventId, _RSU->getSystemPoint()->getGTTPoint()->m_RSUId, clusterIdx, patternIdx, -1, -1, -1, "Succeed");

						//�ͷŵ�����Ϣ������ڴ���Դ
						Delete::safeDelete(info);

						_RSU->getICC_DRAPoint()->m_PatternIsAvailable[clusterIdx][patternIdx] = true;

					}
					else {//��������δ���꣬����ѹ��m_ScheduleInfoTable
						_RSU->getICC_DRAPoint()->pushToScheduleInfoTable(info);
						info = nullptr;
					}
				}
				//������󣬽���pattern�ϵ�������գ���ʱҪ��������ǿգ�Ҫ������nullptrָ�룩
				lst.clear();
			}
		}
	}
}

void RRM_ICC_DRA::writeTTILogInfo(ofstream& t_File, int t_TTI, EventLogType t_EventLogType, int t_EventId, int t_FromRSUId, int t_FromClusterIdx, int t_FromPatternIdx, int t_ToRSUId, int t_ToClusterIdx, int t_ToPatternIdx, std::string t_Description) {
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


void RRM_ICC_DRA::writeClusterPerformInfo(bool isLocationUpdate, ofstream& t_File) {
	if (!isLocationUpdate) return;
	t_File << "[ TTI = " << left << setw(3) << getContext()->m_TTI << "]" << endl;
	t_File << "{" << endl;

	//��ӡVeUE��Ϣ
	t_File << "    VUE Info: " << endl;
	t_File << "    {" << endl;
	for (int VeUEId = 0; VeUEId < getContext()->m_Config.VeUENum; VeUEId++) {
		RRM_VeUE &_VeUE = *(m_VeUEAry[VeUEId]);
		t_File << _VeUE.getICC_DRAPoint()->toString(2) << endl;
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
		t_File << _RSU->getICC_DRAPoint()->toString(2) << endl;
	}
	t_File << "    }" << endl;

	t_File << "}\n\n";
}


pair<int, int> RRM_ICC_DRA::getOccupiedSubCarrierRange(int t_PatternIdx) {
	pair<int, int> res;

	res.first = ns_RRM_ICC_DRA::gc_RBNumPerPattern * t_PatternIdx;
	res.second = ns_RRM_ICC_DRA::gc_RBNumPerPattern * (t_PatternIdx + 1) - 1;

	res.first *= 12;
	res.second = (res.second + 1) * 12 - 1;
	return res;
}
