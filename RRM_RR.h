#pragma once
#include<vector>
#include<list>
#include<thread>
#include<stdexcept>
#include"RRM.h"

//<RRM_RR>: Radio Resource Management Round-Robin


class RRM_RR_VeUE :public RRM_VeUE {
	/*------------------方法------------------*/
public:
	/*
	* 构造函数
	*/
	RRM_RR_VeUE();

	/*
	* 生成格式化字符串
	*/
	std::string toString(int t_NumTab);

	/*
	* 用于取得指向实际类型的指针
	*/
	RRM_TDM_DRA_VeUE *const getTDM_DRAPoint()override { throw std::logic_error("RuntimeException"); }
	RRM_ICC_DRA_VeUE *const getICC_DRAPoint()override { throw std::logic_error("RuntimeException"); }
	RRM_RR_VeUE *const getRRPoint()override { return this; }
};


class RRM_RR_RSU :public RRM_RSU {
	/*------------------域------------------*/
public:
	/*
	* RSU级别的接入列表
	* 外层下标为簇编号
	* 内层list存放的是处于等待接入状态的VeUEId
	*/
	std::vector<std::list<int>> m_AccessEventIdList;

	/*
	* RSU级别的等待列表
	* 外层下标为簇编号
	* 内层list存放的是处于等待接入状态的VeUEId
	* 其来源有：
	*		1、分簇后，由System级的切换链表转入该RSU级别的等待链表
	*		2、事件链表中当前的事件触发，转入等待链表
	*/
	std::vector<std::list<int>> m_WaitEventIdList;

	/*
	* 存放调度调度信息，本次进行传输
	* 外层下标代表簇编号
	* 内层下标代表Pattern编号
	*/
	std::vector<std::vector<ScheduleInfo*>> m_TransimitScheduleInfoTable;

	/*------------------方法------------------*/
public:
	/*
	* 构造函数
	*/
	RRM_RR_RSU();

	/*
	* 初始化
	* 部分成员需要等到GTT模块初始化完毕后，有了簇的数量才能进行本单元RSU的初始化
	*/
	void initialize() override;

	/*
	* 生成格式化字符串
	*/
	std::string toString(int t_NumTab);

	/*
	* 将AccessVeUEIdList的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToAccessEventIdList(int t_ClusterIdx, int t_EventId);

	/*
	* 将WaitVeUEIdList的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToWaitEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId);

	/*
	* 将SwitchVeUEIdList的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToSwitchEventIdList(int t_EventId, std::list<int>& t_SwitchVeUEIdList);

	/*
	* 将ScheduleInfoTable的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToTransimitScheduleInfoTable(ScheduleInfo* t_Info);

	/*
	* 用于取得指向实际类型的指针
	*/
	RRM_TDM_DRA_RSU *const getTDM_DRAPoint()override { throw std::logic_error("RuntimeException"); }
	RRM_ICC_DRA_RSU *const getICC_DRAPoint() override { throw std::logic_error("RuntimeException"); }
	RRM_RR_RSU *const getRRPoint() override { return this; }
};


class RRM_RR :public RRM {
	/*------------------静态------------------*/
public:
	/*
	* 每个Pattern的RB数量
	*/
	static const int s_RB_NUM_PER_PATTERN = 10;

	/*
	* 总的Pattern数量
	*/
	static const int s_TOTAL_PATTERN_NUM = s_TOTAL_BANDWIDTH / s_BANDWIDTH_OF_RB / s_RB_NUM_PER_PATTERN;
	/*------------------域------------------*/
public:
	/*
	* 用于存放进行RSU切换的车辆，暂时保存的作用
	*/
	std::list<int> m_SwitchEventIdList;

	/*
	* 用于存放指定车辆指定Pattern的干扰列表(只保留RSU内簇间干扰)
	* 外层下标为VeUEId
	* 内层下标为PatternIdx
	*/
	std::vector<std::vector<std::list<int>>> m_InterferenceVec;

	/*
	* 多线程总数
	*/
	int m_ThreadNum;

	/*
	* 多线程容器
	*/
	std::vector<std::thread> m_Threads;

	/*
	* 分配给每个线程的RSUId范围
	*/
	std::vector<std::pair<int, int>> m_ThreadsRSUIdRange;
public:
	/*
	* 默认构造函数定义为删除
	*/
	RRM_RR() = delete;

	/*
	* 构造函数
	* 该构造函数定义了该模块的视图
	* 所有指针成员拷贝系统类中的对应成员指针，共享同一实体
	*/
	RRM_RR(System* t_Context);

	/*
	* 初始化RSU VeUE内该单元的内部类
	*/
	void initialize() override;

	/*
	* 当发生位置更新时，清除缓存的调度相关信息
	*/
	void cleanWhenLocationUpdate()override;

	/*
	* RRM_RR调度总控，覆盖基类的虚函数
	*/
	void schedule() override;

private:
	/*
	* 资源分配信息清空
	*/
	void informationClean();

	/*
	* 更新等待链表
	*/
	void updateAccessEventIdList(bool t_ClusterFlag);

	/*
	* 处理事件链表
	* 将该时刻触发的事件放入RSU的等待链表中
	*/
	void processEventList();

	/*
	* 地理位置更新时，处理等待链表
	*/
	void processWaitEventIdListWhenLocationUpdate();

	/*
	* 地理位置更新时，处理转接表
	*/
	void processSwitchListWhenLocationUpdate();

	/*
	* 处理等待链表，生成接入链表
	*/
	void processWaitEventIdList();

	/*
	* 轮询调度，分配当前TTI的资源(就是更新ScheduleTable)
	*/
	void roundRobin();

	/*
	* 时延统计
	*/
	void delaystatistics();

	/*
	* 计算干扰信道响应
	*/
	void transimitPreparation();

	/*
	* 模拟传输开始，更新调度信息，累计吞吐量
	*/
	void transimitStart();
	void transimitStartThread(int t_FromRSUId, int t_ToRSUId);

	/*
	* 模拟传输结束，更新状态
	*/
	void transimitEnd();

	/*
	* 记录调度信息日志
	*/
	void writeScheduleInfo(std::ofstream& t_File);

	/*
	* 以TTI为主键记录日志
	*/
	void writeTTILogInfo(std::ofstream& t_File, int t_TTI, EventLogType t_EventLogType, int t_EventId, int t_FromRSUId, int t_FromClusterIdx, int t_FromPatternIdx, int t_ToRSUId, int t_ToClusterIdx, int t_ToPatternIdx, std::string t_Description);

	/*
	* 写入分簇信息的日志
	*/
	void writeClusterPerformInfo(bool isLocationUpdate, std::ofstream& t_File);

	/*
	* 返回指定Pattern编号对应的子载波编号
	*/
	std::pair<int, int> getOccupiedSubCarrierRange(int t_PatternIdx);
};

