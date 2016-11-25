/*
* =====================================================================================
*
*       Filename:  IMTA.cpp
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

#include<cmath>
#include<memory.h>
#include<iostream>
#include"IMTA.h"
#include"Function.h"

using namespace std;

const double IMTA::m_sacfConstantUMiLoS[25] =
{
	0.753065949852806f, 0.241023875447849f, 0.454091158552085f, -0.097177920212920f, -0.398944655540474f,
	0.241023875447849f, 0.929354051080552f, 0.137998056490968f, -0.242351266621617f, -0.020759074542993f,
	0.454091158552085f, 0.137998056490968f, 0.861515602815864f, -0.175603398954279f, -0.041377149612582f,
	-0.097177920212920f, -0.242351266621617f, -0.175603398954279f, 0.915728740196339f, 0.249853229004786f,
	-0.398944655540474f, -0.020759074542993f, -0.041377149612582f, 0.249853229004786f, 0.881063855850205f
};
const double IMTA::m_sacfConstantUMiNLoS[25] =
{
	0.913514893522226f, 0.0f, 0.178007039816570f, -0.365792336017060f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.178007039816570f, 0.0f, 0.967794909860288f, -0.178007039816570f, 0.0f,
	-0.365792336017060f, 0.0f, -0.178007039816570f, 0.913514893522227f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double IMTA::m_sacfConstantUMiO2I[25] =
{
	0.896950683840360f, 0.241943793527554f, 0.223605545793171f, -0.294861376620174f, 0.0f,
	0.241943793527554f, 0.959179465107684f, -0.030708888757422f, 0.143160464655990f, 0.0f,
	0.223605545793171f, -0.030708888757422f, 0.973505191933743f, 0.036676495468768f, 0.0f,
	-0.294861376620174f, 0.143160464655990f, 0.036676495468768f, 0.944042734529146f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double IMTA::m_sacfConstantSMaLoS[25] =
{
	0.854017192528818f, -0.040746823503881f, 0.423846524961968f, -0.298912118384658f, 0.0f,
	-0.040746823503881f, 0.958761974312789f, -0.023404035164252f, -0.280298812206218f, 0.0f,
	0.423846524961968f, -0.023404035164252f, 0.879509345569223f, -0.215103894600008f, 0.0f,
	-0.298912118384658f, -0.280298812206218f, -0.215103894600008f, 0.886461750943879f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double IMTA::m_sacfConstantSMaNLoS[25] =
{
	0.888863320019977f, -0.028738390905236f, 0.394136210972923f, -0.231846394000744f, 0.0f,
	-0.028738390905236f, 0.976874642167262f, 0.011594978528380f, -0.211555181576079f, 0.0f,
	0.394136210972923f, 0.011594978528380f, 0.917601710925213f, 0.050292184084163f, 0.0f,
	-0.231846394000744f, -0.211555181576079f, 0.050292184084163f, 0.948136251262026f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double IMTA::m_sacfConstantUMaLoS[25] =
{
	0.806310951408682f, 0.245016774934418f, 0.479171304494613f, -0.120392914754038f, -0.213845356893992f,
	0.245016774934418f, 0.924083471541761f, -0.108566442433108f, -0.271617534928914f, 0.021766026753229f,
	0.479171304494613f, -0.108566442433108f, 0.825678402680475f, -0.271600920527001f, -0.055644441252067f,
	-0.120392914754038f, -0.271617534928914f, -0.271600920527001f, 0.915216117252956f, -0.018489442540902f,
	-0.213845356893992f, 0.021766026753229f, -0.055644441252067f, -0.018489442540902f, 0.974863190445988f
};
const double IMTA::m_sacfConstantUMaNLoS[25] =
{
	0.913941405256431f, 0.147728073775767f, 0.318005795482914f, -0.204352240055453f, 0.0f,
	0.147728073775767f, 0.913941405256431f, 0.204352240055453f, -0.318005795482914f, 0.0f,
	0.318005795482914f, 0.204352240055453f, 0.923123353576218f, 0.070397088759368f, 0.0f,
	-0.204352240055453f, -0.318005795482914f, 0.070397088759368f, 0.923123353576218f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double IMTA::m_sacfConstantRMaLoS[25] =
{
	0.965925826289068f, 0.0f, 0.0f, -0.258819045102521f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.258819045102521f, 0.0f, 0.0f, 0.965925826289068f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double IMTA::m_sacfConstantRMaNLoS[25] =
{
	0.955557150656242f, -0.173466133506044f, 0.0f, -0.238369529001059f, 0.0f,
	-0.173466133506044f, 0.938008596337461f, 0.0f, 0.300082278256296f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.238369529001059f, 0.300082278256296f, 0.0f, 0.923650688258459f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double IMTA::m_sacfAngleOffset[m_scbySubPathNum] =
{
	7.8016217564146532088488977351458e-4f, -7.8016217564146532088488977351458e-4f,
	0.00246615023306798769219317505587f, -0.00246615023306798769219317505587f,
	0.00434936049596986930569383739507f, -0.00434936049596986930569383739507f,
	0.00648389817115893437827151120494f, -0.00648389817115893437827151120494f,
	0.00895179373347891640004160995158f, -0.00895179373347891640004160995158f,
	0.01186300292580545813435032615342f, -0.01186300292580545813435032615342f,
	0.01543569190463785077831312115652f, -0.01543569190463785077831312115652f,
	0.02003812514214689787266089371302f, -0.02003812514214689787266089371302f,
	0.02652027798405383792135548122719f, -0.02652027798405383792135548122719f,
	0.03761359070972979671228245975171f, -0.03761359070972979671228245975171f
};
const double IMTA::m_sacfMidPathDelayOffset[m_scbyMidPathNum] =
{
	0.0f, 5.0e-9f, 10.0e-9f
};
const int IMTA::m_sacbyMidPathIndex[m_scbySubPathNum] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0
};

IMTA::IMTA() {
	m_pfGain = nullptr;
	m_pfSinAoD = nullptr;
	m_pfCosAoD = nullptr;
	m_pfSinAoA = nullptr;
	m_pfCosAoA = nullptr;
	m_pfPhase = nullptr;
	m_fGainLoS = 0;
	m_fSinAoDLoS = 0;
	m_pfPhaseLoS = nullptr;
	m_fAntGain = 0;
	m_fMaxAttenu = 0; // dBm
	m_byTxAntNum = 0;
	m_byRxAntNum = 0;
	m_bLoS = false;
	m_bBuilt = false;
	m_bEnable = false;
	m_fPLSF = 0.0f;
	m_pfTxAntSpacing = nullptr;
	m_pfTxSlantAngle = nullptr;
	m_pfRxAntSpacing = nullptr;
	m_pfRxSlantAngle = nullptr;
	//FFT������ʼ��
	m_pwFFTIndex = nullptr;
}

IMTA::~IMTA() {
	refresh();
	Delete::safeDelete(m_pfTxAntSpacing, true);
	Delete::safeDelete(m_pfRxAntSpacing, true);
	Delete::safeDelete(m_pfTxSlantAngle, true);
	Delete::safeDelete(m_pfRxSlantAngle, true);
}

bool IMTA::build(double* t_Pl, double t_fFrequency/*Hz*/, Location &t_eLocation, Antenna &t_eAntenna, double t_fVelocity/*km/h*/, double t_fVAngle/*degree*/) {
	m_bBuilt = false;
	m_fAntGain = t_eAntenna.antGain * 0.1f;
	m_byTxAntNum = t_eAntenna.byTxAntNum;
	m_byRxAntNum = t_eAntenna.byRxAntNum;

	
	Delete::safeDelete(m_pfTxAntSpacing, true);
	m_pfTxAntSpacing = new double[m_byTxAntNum];

	Delete::safeDelete(m_pfTxSlantAngle);
	m_pfTxSlantAngle = new double[m_byTxAntNum];

	Delete::safeDelete(m_pfRxAntSpacing, true);
	m_pfRxAntSpacing = new double[m_byRxAntNum];

	Delete::safeDelete(m_pfRxSlantAngle, true);
	m_pfRxSlantAngle = new double[m_byRxAntNum];

	for (int byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++byTempTxAnt) {
		m_pfTxAntSpacing[byTempTxAnt] = t_eAntenna.TxAntSpacing[byTempTxAnt] * gc_PI2;
		m_pfTxSlantAngle[byTempTxAnt] = t_eAntenna.TxSlantAngle[byTempTxAnt] * gc_Degree2PI;
	}
	for (int byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++byTempRxAnt) {
		m_pfRxAntSpacing[byTempRxAnt] = t_eAntenna.RxAntSpacing[byTempRxAnt] * gc_PI2;
		m_pfRxSlantAngle[byTempRxAnt] = t_eAntenna.RxSlantAngle[byTempRxAnt] * gc_Degree2PI;
	}
	m_fTxAngle = t_eAntenna.TxAngle * gc_Degree2PI;
	m_fRxAngle = t_eAntenna.RxAngle * gc_Degree2PI;
	m_fVelocity = t_fVelocity / 3.6f * t_fFrequency * gc_PI2 / gc_C;
	m_fvAngle = t_fVAngle * gc_Degree2PI;
	//FFT���
	m_wFFTNum = 1;
	m_byFFTOrder = 0;
	m_wHNum = 666;
	while (true)
	{
		if (m_wHNum > m_wFFTNum)
		{
			m_wFFTNum <<= 1;
			++m_byFFTOrder;
		}
		else
		{
			m_fFFTTime = 1.0f / 15000 / m_wFFTNum;
			break;
		}
	}

	double fTemp;
	double fSFSTD;
	double fPL1;
	double fPL2;
	double fDistanceBP = 4 * (t_eLocation.VeUEAntH - 1)*(t_eLocation.RSUAntH - 1)*t_fFrequency / gc_C;

	switch (t_eLocation.locationType)
	{
	case Los:
		m_bLoS = true;
		if (3 < t_eLocation.distance&&t_eLocation.distance < fDistanceBP)
		{
			m_fPLSF = 22.7f * log10(t_eLocation.distance) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);//ת��ΪGHz
		}
		else
		{
			if (fDistanceBP < t_eLocation.distance&&t_eLocation.distance < 5000)
			{
				m_fPLSF = 40.0f * log10(t_eLocation.distance) + 7.56f - 17.3f * log10(t_eLocation.VeUEAntH - 1) - 17.3 * log10(t_eLocation.RSUAntH -1) + 2.7f *(log10(t_fFrequency) - 9.0f);
			}
			else if (t_eLocation.distance<3)
			{
				m_fPLSF = 22.7f * log10(3) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
			}
		}
		break;
	case Nlos:
		fTemp = (2.8f - 0.0024f * t_eLocation.distance1) > 1.84f ? (2.8f - 0.0024f * t_eLocation.distance1) : 1.84f;
		if (3 < t_eLocation.distance1&&t_eLocation.distance1 < fDistanceBP)
		{
			fPL1 = 22.7f * log10(t_eLocation.distance1) + 27.0f + 20.0f *(log10(t_fFrequency) - 9.0f);
		}
		else
		{
			if (fDistanceBP < t_eLocation.distance1&&t_eLocation.distance1 < 5000)
			{
				fPL1 = 40.0f * log10(t_eLocation.distance1) + 7.56f - 17.3f * log10(t_eLocation.VeUEAntH - 1) - 17.3 * log10(t_eLocation.RSUAntH - 1) + 2.7f * (log10(t_fFrequency) - 9.0f);
			}
			else if (t_eLocation.distance1<3)
			{
				fPL1 = 22.7f * log10(3) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
			}
		}
		fPL1 = fPL1 + 17.3f - 12.5f*fTemp + 10 * fTemp * log10(t_eLocation.distance2) + 3 * (log10(t_fFrequency) - 9.0f);
		fTemp = (2.8f - 0.0024f * t_eLocation.distance2) > 1.84f ? (2.8f - 0.0024f * t_eLocation.distance2) : 1.84f;
		if (3 < t_eLocation.distance2&&t_eLocation.distance2 < fDistanceBP)
		{
			fPL2 = 22.7f * log10(t_eLocation.distance2) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
		}
		else
		{
			if (fDistanceBP < t_eLocation.distance2&&t_eLocation.distance2 < 5000)
			{
				fPL2 = 40.0f * log10(t_eLocation.distance2) + 7.56f - 17.3f * log10(t_eLocation.VeUEAntH - 1) - 17.3 * log10(t_eLocation.RSUAntH - 1) + 2.7f * (log10(t_fFrequency) - 9.0f);
			}
			else if (t_eLocation.distance2 < 3)
			{
				fPL2 = 22.7f * log10(3) + 27.0f + 20.0f *(log10(t_fFrequency) - 9.0f);
			}
		}
		fPL2 = fPL2 + 17.3f - 12.5f*fTemp + 10 * fTemp * log10(t_eLocation.distance1) + 3 * (log10(t_fFrequency) - 9.0f);
		m_fPLSF = fPL1 < fPL2 ? fPL1 : fPL2;
	default:
		break;
	}

	fSFSTD = 3.0f;
	double fDSMean;
	double fDSSTD;
	double fASDMean;
	double fASDSTD;
	double fASAMean;
	double fASASTD;
	double fKMean;
	double fKSTD;
	const double *cpfConstant;

	if (t_eLocation.manhattan)
	{
		fDSMean = -7.19f;
		fDSSTD = 0.40f;
		fASDMean = 1.20f;
		fASDSTD = 0.43f;
		fASAMean = 1.75f;
		fASASTD = 0.19f;
		fKMean = 9.0f;
		fKSTD = 5.0f;
		cpfConstant = m_sacfConstantUMiLoS;
		m_fDSRatio = 3.2f;
		m_fXPR = 9.0f;
		m_byPathNum = 12;
		m_fAoDRatio = 3.0f;
		m_fAoARatio = 17.0f;
		m_fPathShadowSTD = 3.0f;
		m_fC = 1.146f;
	}
	else
	{
		fDSMean = -6.89f;
		fDSSTD = 0.54f;
		fASDMean = 1.41f;
		fASDSTD = 0.17f;
		fASAMean = 1.84f;
		fASASTD = 0.15f;
		fKMean = 0.0f;
		fKSTD = 0.0f;
		cpfConstant = m_sacfConstantUMiNLoS;
		m_fDSRatio = 3.0f;
		m_fXPR = 8.0f;
		m_byPathNum = 19;
		m_fAoDRatio = 10.0f;
		m_fAoARatio = 22.0f;
		m_fPathShadowSTD = 3.0f;
		m_fC = 1.273f;
	}

	m_fXPR = pow(10.0f, m_fXPR * -0.05f);
	m_fPathShadowSTD /= 10.0f;

	double afTemp[5] = { 0.0f };
	for (int byTemp = 0; byTemp != 5; ++byTemp)
	{
		for (int byTempTime = 0; byTempTime != 5; ++byTempTime)
		{
			afTemp[byTemp] += (cpfConstant[byTemp * 5 + byTempTime] * t_eLocation.posCor[byTempTime]);
		}
	}
	m_fDS = pow(10.0f, fDSSTD * afTemp[0] + fDSMean);
	m_fAoD = pow(10.0f, fASDSTD * afTemp[1] + fASDMean);
	m_fAoD = m_fAoD < 104.0f ? m_fAoD : 104.0f;
	m_fAoA = pow(10.0f, fASASTD * afTemp[2] + fASAMean);
	m_fAoA = m_fAoA < 104.0f ? m_fAoA : 104.0f;
	m_fAoD *= gc_Degree2PI;
	m_fAoA *= gc_Degree2PI;
	m_fPLSF += fSFSTD * afTemp[3];
	*t_Pl = pow(10, -m_fPLSF / 10);
	m_fKDB = fKSTD * afTemp[4] + fKMean;
	m_fK = pow(10.0f, m_fKDB * 0.1f);
	m_fDS *= -m_fDSRatio;

	if (m_bLoS)
	{
		m_fC *= (1.1035f - 0.028f * m_fKDB - 0.002f * pow(m_fKDB, 2.0f) + 0.0001f * pow(m_fKDB, 3.0f));
		m_fD = 0.7705f - 0.0433f * m_fKDB + 0.0002f * pow(m_fKDB, 2.0f) + 0.000017f * pow(m_fKDB, 3.0f);
		m_fSinAoALoS = sin(m_fTxAngle);
		m_fCosAoALoS = cos(m_fTxAngle - m_fvAngle) * m_fVelocity;
		m_fSinAoDLoS = sin(m_fRxAngle);
	}

	//m_fSinAoALoS = sin(m_fRxAngle);
	//m_fCosAoALoS = cos(m_fRxAngle);

	m_bBuilt = true;

	return true;
}

bool IMTA::enable(bool *t_pbEnable)
{
	//if (m_bBuilt == false)
	//{
	//	return false;
	//}
	refresh();
    
	if (m_bLoS)
	{
		m_pfPhaseLoS = new double[m_byTxAntNum * m_byRxAntNum * 2];
	}

	m_pfGain = new double[m_byPathNum * m_scbySubPathNum];
	m_pfSinAoD = new double[m_byPathNum * m_scbySubPathNum];
	m_pfCosAoD = new double[m_byPathNum * m_scbySubPathNum];
	m_pfPhase = new double[m_byTxAntNum * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2];	
	m_pfSinAoA = new double[m_byPathNum * m_scbySubPathNum];
	m_pfCosAoA = new double[m_byPathNum * m_scbySubPathNum];
	m_pwFFTIndex = new int[m_byPathNum];

	double *pfPathDelay = new double[m_byPathNum];
	double *pfPathPower = new double[m_byPathNum];
	double fPowerTotal = 0.0f;
	double *pfAoD = new double[m_byPathNum];
	double *pfAoA = new double[m_byPathNum];
//	int abyIndex[m_scbySubPathNum];
	double *pfXAoD = new double[m_byPathNum];
	double *pfXAoA = new double[m_byPathNum];
	double fPowerMax;
	int byStoreIndex = 0;
	randomUniform(pfPathDelay, m_byPathNum, 1.0f, 0.0f, true);
	for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathDelay[byTempPath] = m_fDS * log(pfPathDelay[byTempPath]);
	}
	sortBubble(pfPathDelay, m_byPathNum, false, false); //��С����
	for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathDelay[m_byPathNum - 1 - byTempPath] -= pfPathDelay[0];
	}

	randomGaussian(pfPathPower, m_byPathNum, 0.0f, m_fPathShadowSTD);
	for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathPower[byTempPath] = exp((m_fDSRatio - 1.0f) * pfPathDelay[byTempPath] / m_fDS) * pow(10.0f, pfPathPower[byTempPath]);
		fPowerTotal += pfPathPower[byTempPath];
	}
	for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathPower[byTempPath] /= fPowerTotal;
	}

	selectMax(pfPathPower, static_cast<int>(m_byPathNum), &m_byPathFirst, &m_byPathSecond);

		if (m_bLoS)
	{
		for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
		{
			pfPathPower[byTempPath] /= (1.0f + m_fK);
		}
		pfPathPower[0] += (m_fK / (1.0f + m_fK));
		fPowerMax = pfPathPower[0] > pfPathPower[m_byPathFirst] ? pfPathPower[0] : pfPathPower[m_byPathFirst];
	}
	else
	{
		fPowerMax = pfPathPower[m_byPathFirst];
	}
	    
		randomUniform(pfXAoD, m_byPathNum, 1.0f, -1.0f, true);
	    randomUniform(pfXAoA, m_byPathNum, 1.0f, -1.0f, true);
		randomGaussian(pfAoD, m_byPathNum, 0.0f, m_fAoD / 7.0f);//��winner 2 �ǳ���5.0f
		randomGaussian(pfAoA, m_byPathNum, 0.0f, m_fAoA / 7.0f);
		

		for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
		{
			if (pfXAoD[byTempPath] > 0.0f)
			{
				pfAoD[byTempPath] += (2.0f * m_fAoD * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			else
			{
				pfAoD[byTempPath] -= (2.0f * m_fAoD * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			if (pfXAoA[byTempPath] > 0.0f)
			{
				pfAoA[byTempPath] += (2.0f * m_fAoA * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			else
			{
				pfAoA[byTempPath] -= (2.0f * m_fAoA * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			pfAoD[byTempPath] += m_fTxAngle;
            pfAoA[byTempPath] += m_fRxAngle;

		}

	if (m_bLoS)
	{
		pfPathPower[0] -= (m_fK / (1.0f + m_fK));
		for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
		{
			pfAoD[m_byPathNum - 1 - byTempPath] = pfAoD[m_byPathNum - 1 - byTempPath] - pfAoD[0] + m_fTxAngle;
			pfAoA[m_byPathNum - 1 - byTempPath] = pfAoA[m_byPathNum - 1 - byTempPath] - pfAoA[0] + m_fRxAngle;
			pfPathDelay[byTempPath] /= m_fD;
		}
	}


	for (int byTempPath = 0; byTempPath != m_byPathNum; ++byTempPath)
	{
		pfPathPower[byTempPath] /= m_scbySubPathNum;
		m_pwFFTIndex[byStoreIndex] = (int)floor(pfPathDelay[byTempPath] / m_fFFTTime + 0.5f);
		if (m_pwFFTIndex[byStoreIndex] >= m_wFFTNum)
		{
			m_pwFFTIndex[byStoreIndex] = m_wFFTNum - 1;
		}
		++byStoreIndex;
	}

		for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
 		{
      		for (int byTempSubPath = 0; byTempSubPath != m_scbySubPathNum; ++ byTempSubPath)
		    {
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] = 0.6f;//m_fAntGain
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] = pow(10.0f, m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath]);
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] *= pfPathPower[byTempPath];
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] = sqrt(m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath]);
				//if (m_bLoS)
	   //        { 
    //            m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath]/=sqrt(1.0f / (1.0f + m_fK);//ǰ���ڼ���Pnʱ���Ѿ������ˣ��������Ҫ��ȥ��
				//}
			    m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath] = pfAoD[byTempPath] + m_sacfAngleOffset[byTempSubPath] * m_fAoDRatio;
				m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath] = sin(m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath]);
			   
				//m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = pfAoA[byTempPath] + m_sacfAngleOffset[abyIndex[byTempSubPath]] * m_fAoARatio;
			   	m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = pfAoA[byTempPath] + m_sacfAngleOffset[byTempSubPath] * m_fAoARatio;
				m_pfCosAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = cos(m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] - m_fvAngle) * m_fVelocity;
		     	m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = sin(m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath]);
				
			}
	}


		if (m_bLoS)
	{
	     	m_fGainLoS= m_fMaxAttenu;
			m_fGainLoS = sqrt(m_fK / (1.0f + m_fK) * pow(10.0f, m_fGainLoS));
		}


	double *pfPhasePol = new double[m_byPathNum * m_scbySubPathNum * 4];
	double *pfSlantVV = new double[m_byTxAntNum * m_byRxAntNum];
	double *pfSlantVH = new double[m_byTxAntNum * m_byRxAntNum];
	double *pfSlantHV = new double[m_byTxAntNum * m_byRxAntNum];
	double *pfSlantHH = new double[m_byTxAntNum * m_byRxAntNum];
	for (int byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
	{
		for (int byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
		{
			pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = sin(m_pfTxSlantAngle[byTempTxAnt]) * sin(m_pfRxSlantAngle[byTempRxAnt]);
			pfSlantVH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = sin(m_pfTxSlantAngle[byTempTxAnt]) * cos(m_pfRxSlantAngle[byTempRxAnt]) * m_fXPR;
			pfSlantHV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = cos(m_pfTxSlantAngle[byTempTxAnt]) * sin(m_pfRxSlantAngle[byTempRxAnt]) * m_fXPR;
			pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = cos(m_pfTxSlantAngle[byTempTxAnt]) * cos(m_pfRxSlantAngle[byTempRxAnt]);
		}
	}

	randomUniform(pfPhasePol, m_byPathNum * m_scbySubPathNum * 4, gc_PI, gc_PINeg, false);
	for (int byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
	{
		for (int byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
		{
				for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
				{
					for (int byTempSubPath = 0; byTempSubPath != m_scbySubPathNum; ++ byTempSubPath)
					{
						m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2] =
							pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4]) +
							pfSlantVH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 1]) +
							pfSlantHV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 2]) +
							pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 3]);
						m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2 + 1] =
							pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4]) +
							pfSlantVH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 1]) +
							pfSlantHV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 2]) +
							pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 3]);
					}
				}
			}
		}

	if (m_bLoS)
		{
			randomUniform(pfPhasePol, 1, gc_PI, gc_PINeg, false);
			for (int byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
			{
				for (int byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
				{
					m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2] =
						(pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] +
						pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt]) *
						cos(pfPhasePol[0]);
					m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2 + 1] =
						(pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] +
						pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt]) *
						sin(pfPhasePol[0]);
				}
			}
		}


	Delete::safeDelete(pfPhasePol, true);
	Delete::safeDelete(pfSlantVV, true);
	Delete::safeDelete(pfSlantVH, true);
	Delete::safeDelete(pfSlantHV, true);
	Delete::safeDelete(pfSlantHH, true);
	Delete::safeDelete(pfPathDelay, true);
	Delete::safeDelete(pfPathPower, true);
	Delete::safeDelete(pfXAoD, true);
	Delete::safeDelete(pfXAoA, true);
	Delete::safeDelete(pfAoD, true);
	Delete::safeDelete(pfAoA, true);
	return true;
}

void IMTA::calculate(double* t_HAfterFFT, double t_fT/*s */, double *t_pfTemp, double *t_pfSin, double *t_pfCos,double *t_pfH,double *t_pfHFFT)
{
	double fCos;
	double fSin;

	memset(t_pfH, 0, m_byTxAntNum * m_byRxAntNum * m_byPathNum * 2 * sizeof(double));

		for (int byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
		{
			for (int byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
			{
				for (int byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
				{
						for (int byTempSubPath = 0; byTempSubPath != m_scbySubPathNum; ++ byTempSubPath)
						{
							t_pfTemp[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] =
						            m_pfTxAntSpacing[byTempTxAnt] * m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath] +
							         m_pfRxAntSpacing[byTempRxAnt] * m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] +
							         m_pfCosAoA[byTempPath * m_scbySubPathNum + byTempSubPath] * t_fT;//t_fTΪʱ��
						
							t_pfCos[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] =
								   cos(t_pfTemp[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath]*2*gc_PI*gc_C/gc_FC);
							
							t_pfSin[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] =
							       sin(t_pfTemp[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath]*2*gc_PI*gc_C/gc_FC);

							t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2 ] +=
								m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] *
								(t_pfCos[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2] -
								t_pfSin[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2+ 1]);
							t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2 + 1] +=
								m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] *
								(t_pfSin[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2] +
								t_pfCos[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2+ 1]);
				    }


                    if (m_bLoS && byTempPath == 0)
				   {
					fSin = m_pfRxAntSpacing[byTempRxAnt] * m_fSinAoDLoS + m_pfTxAntSpacing[byTempTxAnt] * m_fSinAoALoS + m_fCosAoALoS * t_fT;
					fCos = cos(fSin);
					fSin = sin(fSin);
					t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2] +=
					m_fGainLoS *
					(fCos * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2] -
					 fSin * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2 + 1]);
				    t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2+ 1] +=
					m_fGainLoS *
					(fSin * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2] +
					fCos * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2 + 1]);
				    }
				}
			}
		}


		memset(t_pfHFFT, 0, m_byTxAntNum * m_byRxAntNum * 1024 * 2 * sizeof(double));

		for (int byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++byTempTxAnt)
		{
			for (int byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++byTempRxAnt)
			{
				for (int byTempPath = 0; byTempPath != m_byPathNum; ++byTempPath)
				{
					t_pfHFFT[byTempTxAnt * m_byRxAntNum * m_wFFTNum * 2 + byTempRxAnt * m_wFFTNum * 2 + m_pwFFTIndex[byTempPath] * 2] +=
						t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum * 2 + byTempRxAnt * m_byPathNum * 2 + byTempPath * 2];
					t_pfHFFT[byTempTxAnt * m_byRxAntNum * m_wFFTNum * 2 + byTempRxAnt * m_wFFTNum * 2 + m_pwFFTIndex[byTempPath] * 2 + 1] +=
						t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum * 2 + byTempRxAnt * m_byPathNum * 2 + byTempPath * 2 + 1];
				}
				
			}
		}


		//����fftw�ṩ�ĺ�������fft�任
		fftw_complex *in1, *out1, *in2, *out2;
		fftw_plan ptemp1, ptemp2;
		in1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_wFFTNum);
		out1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_wFFTNum);
		in2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_wFFTNum);
		out2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_wFFTNum);
		for (int tempnum = 0; tempnum<1024; tempnum++)
		{
			in1[tempnum][0] = t_pfHFFT[tempnum * 2];
			in1[tempnum][1] = t_pfHFFT[tempnum * 2 + 1];
			in2[tempnum][0] = t_pfHFFT[tempnum * 2 + 2048];
			in2[tempnum][1] = t_pfHFFT[tempnum * 2 + 2049];
		}
		ptemp1 = fftw_plan_dft_1d(1024, in1, out1, FFTW_FORWARD, FFTW_ESTIMATE);
		ptemp2 = fftw_plan_dft_1d(1024, in2, out2, FFTW_FORWARD, FFTW_ESTIMATE);
		fftw_execute(ptemp1);
		fftw_execute(ptemp2);


		int HAfterFFT_Iter = 0;
		for (int row = 0; row < 1024; row++) {
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out1[row][0];
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out1[row][1];
		}
		for (int row = 0; row < 1024; row++) {
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out2[row][0];
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out2[row][1];
		}
	
		fftw_destroy_plan(ptemp1);
		fftw_destroy_plan(ptemp2);
		fftw_free(in1);
		fftw_free(in2);
		fftw_free(out1);
		fftw_free(out2);
	return;
}

void IMTA::refresh(){
	Delete::safeDelete(m_pfGain, true);
	Delete::safeDelete(m_pfSinAoD, true);
	Delete::safeDelete(m_pfCosAoD, true);
	Delete::safeDelete(m_pfPhase, true);
	Delete::safeDelete(m_pfSinAoA, true);
	Delete::safeDelete(m_pfCosAoA, true);
	Delete::safeDelete(m_pfPhaseLoS, true);
	Delete::safeDelete(m_pwFFTIndex, true);
}