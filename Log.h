#pragma once
#include<fstream>
#include<vector>
#include"Enumeration.h"


/*===========================================
*            �����־�ļ�������
* ==========================================*/
extern std::ofstream g_FileTemp;
//GTT_Urbanģ��
extern std::ofstream g_FileVeUELocationUpdateLogInfo;
extern std::ofstream g_FileVeUENumPerRSULogInfo;
extern std::ofstream g_FileLocationInfo;
extern std::ofstream g_FileVeUEMessage;
//RRMģ��
extern std::ofstream g_FileScheduleInfo;
extern std::ofstream g_FileClasterPerformInfo;
extern std::ofstream g_FileEventListInfo;
extern std::ofstream g_FileTTILogInfo;
extern std::ofstream g_FileEventLogInfo;
//TMC_Bģ��
extern std::ofstream g_FileStatisticsDescription;
extern std::ofstream g_FileEmergencyDelayStatistics;
extern std::ofstream g_FilePeriodDelayStatistics;
extern std::ofstream g_FileDataDelayStatistics;
extern std::ofstream g_FileEmergencyPossion;
extern std::ofstream g_FileDataPossion;
extern std::ofstream g_FileEmergencyConflictNum;
extern std::ofstream g_FilePeriodConflictNum;
extern std::ofstream g_FileDataConflictNum;
extern std::ofstream g_FileTTIThroughput;
extern std::ofstream g_FileRSUThroughput;
extern std::ofstream g_FilePackageLoss;
extern std::ofstream g_FilePackageTransimit;

void logFileConfig(Platform t_Platform);




