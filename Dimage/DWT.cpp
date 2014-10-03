#pragma once

#include "stdafx.h"
#include "CImg.h"
#include "math.h"

int Log2(int n)
{
	int rsl = 0;
	while (n >>= 1) rsl++;
	return rsl;
}
BOOL DWTStep_1D(double* pDbSrc, int nCurLevel,int nInv, int nStep);

//DWTstep_2d(Dwttemp, nMaxWLevel-Curdpth, nMaxHLevel-Curdpth,nMaxWLevel, nMaxHLevel, Inv)
BOOL DWTStep_2D(double* pDbSrc, int nCurWLevel, int nCurHLevel,int nMaxWLevel, int nMaxHLevel, int Inv)
{
	// ����ͼ��ĳ��ȺͿ�ȣ�2���ݶ��룩
	int W = 1<<nMaxWLevel, H = 1<<nMaxHLevel;
	int nStep=1;
	// ���㵱ǰ�ֽ��ͼ��ĳ��ȺͿ��
	int CurW = 1<<nCurWLevel, CurH = 1<<nCurHLevel;
	int i=0;
	// �ж��ǽ���DWT����IDWT
	if (!Inv)
	{	// ���н���һάDWT
		for (i=0; i<CurH; i++)
		if (!DWTStep_1D(pDbSrc+(int)i*W*nStep, nCurWLevel, Inv, nStep)) return FALSE;
		 //���н���һάDWT
		for (i=0; i<CurW; i++)
			if (!DWTStep_1D(pDbSrc+i*nStep, nCurHLevel, Inv, W*nStep)) return FALSE;
	}

	// �������IDWT
	else
	{
		// ���㵱ǰ�任��ͼ��ĳ��ȺͿ��
		CurW <<= 1;
		CurH <<= 1;

		// ���н���IDWT
		for (i=0; i<CurW; i++)
			if (!DWTStep_1D(pDbSrc+i*nStep, nCurHLevel, Inv, W*nStep)) return FALSE;
		// ���н���IDWT
		for (i=0; i<CurH; i++)
			if (!DWTStep_1D(pDbSrc+(int)i*W*nStep, nCurWLevel, Inv, nStep)) return FALSE;
	}

	// ����
	return TRUE;
}

BOOL DWTStep_1D(double* pDbSrc, int nCurLevel,int nInv, int nStep)
{
	double s = sqrt((double)2);
	
	// ���С������ָ��
	//double* h = (double*)hCoef[nSupp-1]
	double h[2] = { 1/s, 1/s};
	
	// ȷ�ϵ�ǰ������Ч
	//ASSERT(nCurLevel>=0);

	// ���㵱ǰ�����ĳ���
	int CurN = 1<<nCurLevel; //nCurLevel=nMaxLevel-nCurDepth,2��nCurLevel���ݼ���ǰ���У��߶ȣ���С
	if (nInv) CurN <<= 1;  //��Ϊ���任������ΧΪ������ǰ���д�С����

	// ȷ����ѡ���С�����͵�ǰ�����ĳ�����Ч
	/*
	if (nSupp<1 || nSupp>10 || CurN<2*nSupp) 
		return FALSE;
	*///not used,nSupp =====1

	// ������ʱ�ڴ����ڴ�Ž��
	double *ptemp = new double[CurN];
	if (!ptemp) return FALSE;

	double	s1, s2;
	int	Index1, Index2;

	// �ж��ǽ���DWT����IDWT
	if (!nInv)
	{	// DWT
		Index1=0;
		//Index2=2*nSupp-1;
		Index2=1;

		// ���о��������s1Ϊ��Ƶ���֣�s2Ϊ��Ƶ���ֵĽ��
		for (int i=0; i<CurN/2; i++)
		{	
			s1 = s2 = 0;
			double t = -1;
			//for (int j=0; j<2*nSupp; j++, t=-t)
			for (int j=0; j<2; j++, t=-t)//j=0,1with t=-1,t=1;
			{
				//s1 += h[j]*pDbSrc[(Index1 & CurN-1) * nStep];//&not necessary
				//s2 += t*h[j]*pDbSrc[(Index2 & CurN-1) * nStep];
				s1 += h[j]*pDbSrc[ Index1 * nStep];  //nStep=1 in Row clac,nStep=Width in Column
				s2 += t*h[j]*pDbSrc[ Index2 * nStep];

				Index1++;//index1 0,1/2,3/4,5/...
				Index2--;//index2 1,0/3,2/...
			}

			// ������������ʱ�ڴ���
			ptemp[i] = s1/s;
			ptemp[i+CurN/2] = s2/s;

			//Index1 -= 2*nSupp;
			//Index2 += 2*nSupp;
			//Index1 += 2;
			Index2 += 4;
		}
	}

	// �������IDWT
	else
	{	// IDWT
		//Index1 = CurN/2;
		Index1 = 0;
		Index2 = 0;
		
		// ���о������������s1Ϊ��Ƶ���֣�s2Ϊ��Ƶ���ֵĽ��
		for (int i=0; i<CurN/2; i++)
		{

			s1 = h[0]*pDbSrc[Index1 * nStep]+h[1]*pDbSrc[(Index2 + CurN/2) * nStep];  //a0+a128/a1+a129....
			s2 = h[1]*pDbSrc[Index1 * nStep]-h[0]*pDbSrc[(Index2 + CurN/2) * nStep];  //a0-a128/...

			ptemp[2*i] = s1*s; // 0/2/4....
			ptemp[2*i+1] = s2*s; // 1/3/5...

			Index1++;
			Index2++;
		}
	}
	
	// ���������Դͼ����
	for (int i=0; i<CurN; i++)
		pDbSrc[i*nStep] = ptemp[i];

	// �ͷ���ʱ�ڴ棬������
	delete[] ptemp;
	return TRUE;
}