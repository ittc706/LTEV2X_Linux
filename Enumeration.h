#pragma once

enum Platform {
	Windows,
	Linux
};
enum GTTMode {
	URBAN,      //����
	HIGHSPEED   //����
};

enum RRMMode {
	RR,       //Round-Robin,��ѯ
	TDM_DRA,        //Distributed Resource Allocation,�ֲ�ʽ
	ICC_DRA
};

enum WTMode {
	SINR_MRC,
	SINR_MMSE
};

enum LocationType { //λ������ 
	Los,
	Nlos,
	None
};

enum MessageType {
	EMERGENCY = 0,   //������Ϣ�����糵����Ϣ�����ȼ��ϸߣ�
	PERIOD = 1,  //��������Ϣ������VeUE�Ļ�����Ϣ�����٣�λ�ã�����ȵȣ�
	DATA = 2     //����ҵ��(�ı�����Ƶ�������ȵȣ�
};

enum RSUType {
	INTERSECTION,       //����ʮ��·�����룬��������
	VERTICAL,           //������ֱ�ĵ�·��
	HORIZONTAL          //����ˮƽ�ĵ�·��    
};

enum EventLogType {
	TRANSIMITTING,
	WITHDRAWING,
	SUCCEED,
	EVENT_TO_WAIT,
	SCHEDULETABLE_TO_SWITCH,
	SCHEDULETABLE_TO_WAIT,
	WAIT_TO_SWITCH,
	WAIT_TO_WAIT,
	WAIT_TO_ACCESS,
	ACCESS_TO_WAIT,
	SWITCH_TO_WAIT,
	TRANSIMIT_TO_WAIT,
};

enum ModulationType {
	QPSK = 2,
	_16QAM = 4,
	_64QAM = 6,
};
