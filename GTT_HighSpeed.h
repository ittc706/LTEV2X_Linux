#pragma once
#include<stdexcept>
#include"GTT.h"

// <GTT_HighSpeed>: Geographical Topology and Transport HighSpeed


class GTT_HighSpeed_VeUE:public GTT_VeUE {
	/*------------------方法------------------*/
public:
	/*
	* 构造函数
	*/
	GTT_HighSpeed_VeUE() = delete;
	GTT_HighSpeed_VeUE(VeUEConfig &t_VeUEConfig);

	/*
	* 用于取得指向实际类型的指针
	*/
	GTT_Urban_VeUE  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_VeUE  *const getHighSpeedPoint()override { return this; }
};


class  GTT_HighSpeed_RSU :public GTT_RSU {
public:
	/*
	* 构造函数
	*/
	GTT_HighSpeed_RSU();

	/*
	* 用于取得指向实际类型的指针
	*/
	GTT_Urban_RSU  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_RSU  *const getHighSpeedPoint()override { return this; }
};


class  GTT_HighSpeed_eNB :public GTT_eNB {
public:
	/*
	* 初始化方法
	* 不用构造函数的原因是构造的时刻其依赖项还没创建完毕
	*/
	void initialize(eNBConfig &t_eNBConfig)override;

	/*
	* 用于取得指向实际类型的指针
	*/
	GTT_Urban_eNB  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_eNB  *const getHighSpeedPoint()override { return this; }
};

 
class GTT_HighSpeed_Road :public GTT_Road {
public:
	/*
	* 构造函数
	*/
	GTT_HighSpeed_Road(HighSpeedRodeConfig &t_RoadHighSpeedConfig);

	/*
	* 用于取得指向实际类型的指针
	*/
	GTT_Urban_Road  *const getUrbanPoint()override { throw std::logic_error("RuntimeException"); }
	GTT_HighSpeed_Road  *const getHighSpeedPoint()override { return this; }
};


class GTT_HighSpeed :public GTT {
	/*------------------静态------------------*/
public:
	/*
	* 随机数引擎，该类共享
	*/
	static std::default_random_engine s_Engine;

	/*
	* 基站数量
	*/
	static const int s_eNB_NUM = 2;

	/*
	* 道路数量
	*/
	static const int s_ROAD_NUM = 6;

	/*
	* RSU数量
	*/
	static const int s_RSU_NUM = 35;

	/*
	* RSU内簇数量
	*/
	static const int s_RSU_CLUSTER_NUM = 2;

	/*
	* 路长
	*/
	static const int s_ROAD_LENGTH = 3464;

	/*
	* 路宽
	*/
	static const double s_ROAD_WIDTH;

	/*
	* ???
	*/
	static const double s_ISD;

	/*
	* ???
	*/
	static const double s_ROAD_TOPO_RATIO[s_ROAD_NUM * 2];

	/*
	* ???
	*/
	static const double s_RSU_TOPO_RATIO[s_RSU_NUM * 2];

	/*
	* ???
	*/
	static const double s_eNB_TOPO[s_eNB_NUM * 2];
	/*------------------域------------------*/
private:
	/*
	* 高速Rode总数
	*/
	int m_HighSpeedRodeNum;

	/*
	* user per road array
	*/
	int* m_pupr;

	/*
	* 车辆车速
	*/
	double m_Speed;//km/h

	/*
	* 统计每个RSU下的车辆数目
	* 外层下标代表第几次位置更新(从0开始)
	* 内层下标代表RSU编号
	* 仅用于用于统计车辆分布情况
	*/
	std::vector<std::vector<int>> m_VeUENumPerRSU;

	/*------------------方法------------------*/
public:
	/*
	* 默认构造函数定义为删除
	*/
	GTT_HighSpeed() = delete;

	/*
	* 构造函数
	* 这里指针都是引用类型，因为需要初始化系统的各个实体数组
	* 该构造函数也定义了该模块的视图
	*/
	GTT_HighSpeed(System* t_Context);

	/*
	* 模块参数配置
	*/
	void configure()override;

	/*
	* 当发生位置更新时，清除缓存的调度相关信息
	*/
	void cleanWhenLocationUpdate()override;

	/*
	* 初始化各个实体数组
	*/
	void initialize()override;

	/*
	* 信道产生
	*/
	void channelGeneration()override;

	/*
	* 信道刷新
	*/
	void freshLoc() override;

	/*
	* 写入地理位置更新日志
	*/
	void writeVeUELocationUpdateLogInfo(std::ofstream &t_File1, std::ofstream &t_File2) override;

	/*
	* 计算干扰矩阵
	* 传入的参数解释
	*		外层下标为车辆编号
	*		内层下标为Pattern编号
	*		最内层list为该车辆在该Pattern下的干扰列表
	* 目前仅有簇间干扰，因为RSU间干扰太小，几乎可以忽略
	*/
	void calculateInterference(const std::vector<std::vector<std::list<int>>>& t_RRMInterferenceVec) override;
};