#pragma once
#include<vector>
#include<random>
#include<thread>
#include<stdexcept>
#include"RRM.h"

//<RRM_TDM_DRA> :Radio Resource Management Time Division Dultiplexing based Distributed Resource Allocation


class RRM_TDM_DRA_VeUE:public RRM_VeUE {
	/*------------------静态------------------*/
public:
	static std::default_random_engine s_Engine;

	/*------------------域------------------*/
public:
	/*
	* 该VeUE所在簇的当前地理位置的相对调度区间
	*/
	std::tuple<int, int> m_ScheduleInterval;

	/*------------------方法------------------*/
public:
	/*
	* 构造函数
	*/
	RRM_TDM_DRA_VeUE();

	/*
	* 随机选取可用资源快
	*/
	int selectRBBasedOnP2(const std::vector<int>&t_CurAvaliablePatternIdx);

	/*
	* 生成格式化字符串
	*/
	std::string toString(int t_NumTab);

	/*
	* 用于取得指向实际类型的指针
	*/
	RRM_TDM_DRA_VeUE *const getTDM_DRAPoint()override { return this; }
	RRM_ICC_DRA_VeUE *const getICC_DRAPoint()override { throw std::logic_error("RuntimeException"); }
	RRM_RR_VeUE *const getRRPoint()override { throw std::logic_error("RuntimeException"); }
};


class RRM_TDM_DRA_RSU :public RRM_RSU{
	/*------------------域------------------*/
public:
	/*
	* TDR:Time Domain Resource
	* 下标代表簇编号
	* tuple存储的变量的含义依次为：存储每个簇所分配时间数量区间的左端点，右端点以及区间长度
	*/
	std::vector<std::tuple<int, int, int>> m_ClusterTDRInfo;

	/*
	* Pattern块释是否可用的标记
	* 外层下标代表簇编号
	* 内层下标代表Pattern编号
	* 若"m_PatternIsAvailable[i][j]==true"代表簇i的Pattern块j可用
	*/
	std::vector<std::vector<bool>> m_PatternIsAvailable;

	/*
	* 接入列表
	* 存放的是VeUEId
	* 外层下标代表簇编号
	* 内层first代表Emergency，second代表非Emergency
	*/
	std::vector<std::pair<std::list<int>, std::list<int>>> m_AccessEventIdList;

	/*
	* 等待列表
	* 存放的是VeUEId
	* 外层下标代表簇编号
	* 内层first代表Emergency，second代表非Emergency
	*/
	std::vector<std::pair<std::list<int>, std::list<int>>> m_WaitEventIdList;

	/*
	* 存放调度调度信息(已经成功接入，但尚未传输完毕，在其传输完毕之前会一直占用该资源块)
	* 外层下标代表簇编号
	* 内层下标代表Pattern编号
	*/
	std::vector<std::vector<ScheduleInfo*>> m_ScheduleInfoTable;

	/*
	* 当前时刻当前RSU内处于传输状态的调度信息链表
	* 外层下标为簇编号
	* 中层下标代表Pattern编号
	* 内层用list用于处理冲突，即对应簇对应Pattern下的当前进行传输的事件的调度信息
	* 对于紧急事件，所有簇都有效
	* 对于非紧急事件，仅当前时刻对应的簇有效
	*/
	std::vector<std::vector<std::list<ScheduleInfo*>>>  m_TransimitScheduleInfoList;

	/*------------------方法------------------*/
public:
	/*
	* 构造函数
	*/
	RRM_TDM_DRA_RSU();

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
	* 返回当前TTI可以进行事件传输的簇编号
	*/
	int getClusterIdx(int t_TTI);

	/*
	* 将AccessVeUEIdList的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToAccessEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId);

	/*
	* 将WaitVeUEIdList的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToWaitEventIdList(bool t_IsEmergency, int t_ClusterIdx, int t_EventId);

	/*
	* 将SwitchVeUEIdList的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToSwitchEventIdList(std::list<int>& t_SwitchVeUEIdList, int t_EventId);

	/*
	* 将TransimitScheduleInfo的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToTransimitScheduleInfoList(ScheduleInfo* t_Info);

	/*
	* 将ScheduleInfoTable的添加封装起来，便于查看哪里调用，利于调试
	*/
	void pushToScheduleInfoTable(ScheduleInfo* t_Info);

	/*
	* 将RSU级别的ScheduleInfoTable的弹出封装起来，便于查看哪里调用，利于调试
	*/
	void pullFromScheduleInfoTable(int t_TTI);

	/*
	* 用于取得指向实际类型的指针
	*/
	RRM_TDM_DRA_RSU *const getTDM_DRAPoint()override { return this; }
	RRM_ICC_DRA_RSU *const getICC_DRAPoint() override { throw std::logic_error("RuntimeException"); }
	RRM_RR_RSU *const getRRPoint() override { throw std::logic_error("RuntimeException"); }
};


class RRM_TDM_DRA :public RRM {
	/*------------------静态------------------*/
public:
	/*
	* 所有簇进行一次DRA所占用的TTI数量。(NTTI:Number of TTI)
	*/
	static const int s_NTTI = 100;

	/*
	* 事件的Pattern的类型种类
	* 即紧急事件，周期事件，数据业务事件
	*/
	static const int s_PATTERN_TYPE_NUM = 3;

	/*
	* 每个Pattern种类所占的RB数量
	*/
	static const int s_RB_NUM_PER_PATTERN_TYPE[s_PATTERN_TYPE_NUM];

	/*
	* 每个Pattern种类对应的Pattern数量
	*/
	static const int s_PATTERN_NUM_PER_PATTERN_TYPE[s_PATTERN_TYPE_NUM];

	/*
	* 每个种类的事件，其各自的Pattern的开始与结束编号，即[startIdx,endIdx]，闭区间
	*/
	static const int s_PATTERN_TYPE_PATTERN_INDEX_INTERVAL[s_PATTERN_TYPE_NUM][2];

	/*
	* 所有Pattern数量，包括三个事件
	*/
	static const int s_TOTAL_PATTERN_NUM;
	/*------------------域------------------*/
public:
	/*
	* 用于存放进行RSU切换的车辆，暂时保存的作用
	*/
	std::list<int> m_SwitchEventIdList;

	/*
	* 用于存放指定车辆指定Pattern的干扰列表(只保留RSU内簇间干扰)
	* 外层下标为VeUEId
	* 内层下标为PatternIdx(绝对量)
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

	/*------------------方法------------------*/
public:
	/*
	* 默认构造函数定义为删除
	*/
	RRM_TDM_DRA() = delete;

	/*
	* 构造函数
	* 该构造函数定义了该模块的视图
	* 所有指针成员拷贝系统类中的对应成员指针，共享同一实体
	*/
	RRM_TDM_DRA(System* t_Context);

	/*
	* 初始化RSU VeUE内该单元的内部类
	*/
	void initialize() override;

	/*
	* 当发生位置更新时，清除缓存的调度相关信息
	*/
	void cleanWhenLocationUpdate()override;

	/*
	* RRM_TDM_DRA调度总控，覆盖基类的虚函数
	*/
	void schedule() override;


private:
	/*
	* 资源分配信息清空
	*/
	void informationClean();

	/*
	* 基于簇大小的时分复用
	*/
	void groupSizeBasedTDM(bool t_ClusterFlag);

	/*
	* 均匀分配给每个簇时域资源
	*/
	void uniformTDM(bool t_ClusterFlag);

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
	* 地理位置更新时，处理调度表
	*/
	void processScheduleInfoTableWhenLocationUpdate();

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
	* 基于P1/P2/P3的资源选择
	*/
	void selectRBBasedOnP123();

	/*
	* 时延统计
	*/
	void delaystatistics();

	/*
	* 帧听冲突，冲突后避让
	*/
	void conflictListener();

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
	void writeClusterPerformInfo(std::ofstream &t_File);


	/*
	* 返回给定数组中最大值的下标
	*/
	int getMaxIndex(const std::vector<double>&t_ClusterSize);

	/*
	* 返回指定Pattern编号对应的Pattern类型编号，即事件类型编号
	*/
	int getPatternType(int t_PatternIdx);

	/*
	* 返回指定Pattern编号对应的子载波编号
	*/
	std::pair<int, int> getOccupiedSubCarrierRange(MessageType t_MessageType, int t_PatternIdx);

};



