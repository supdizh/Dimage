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
	// 计算图象的长度和宽度（2次幂对齐）
	int W = 1<<nMaxWLevel, H = 1<<nMaxHLevel;
	int nStep=1;
	// 计算当前分解的图象的长度和宽度
	int CurW = 1<<nCurWLevel, CurH = 1<<nCurHLevel;
	int i=0;
	// 判断是进行DWT还是IDWT
	if (!Inv)
	{	// 对行进行一维DWT
		for (i=0; i<CurH; i++)
		if (!DWTStep_1D(pDbSrc+(int)i*W*nStep, nCurWLevel, Inv, nStep)) return FALSE;
		 //对列进行一维DWT
		for (i=0; i<CurW; i++)
			if (!DWTStep_1D(pDbSrc+i*nStep, nCurHLevel, Inv, W*nStep)) return FALSE;
	}

	// 否则进行IDWT
	else
	{
		// 计算当前变换的图象的长度和宽度
		CurW <<= 1;
		CurH <<= 1;

		// 对列进行IDWT
		for (i=0; i<CurW; i++)
			if (!DWTStep_1D(pDbSrc+i*nStep, nCurHLevel, Inv, W*nStep)) return FALSE;
		// 对行进行IDWT
		for (i=0; i<CurH; i++)
			if (!DWTStep_1D(pDbSrc+(int)i*W*nStep, nCurWLevel, Inv, nStep)) return FALSE;
	}

	// 返回
	return TRUE;
}

BOOL DWTStep_1D(double* pDbSrc, int nCurLevel,int nInv, int nStep)
{
	double s = sqrt((double)2);
	
	// 获得小波基的指针
	//double* h = (double*)hCoef[nSupp-1]
	double h[2] = { 1/s, 1/s};
	
	// 确认当前层数有效
	//ASSERT(nCurLevel>=0);

	// 计算当前层数的长度
	int CurN = 1<<nCurLevel; //nCurLevel=nMaxLevel-nCurDepth,2的nCurLevel次幂即当前序列（尺度）大小
	if (nInv) CurN <<= 1;  //若为反变换，处理范围为两倍当前序列大小，？

	// 确认所选择的小波基和当前层数的长度有效
	/*
	if (nSupp<1 || nSupp>10 || CurN<2*nSupp) 
		return FALSE;
	*///not used,nSupp =====1

	// 分配临时内存用于存放结果
	double *ptemp = new double[CurN];
	if (!ptemp) return FALSE;

	double	s1, s2;
	int	Index1, Index2;

	// 判断是进行DWT还是IDWT
	if (!nInv)
	{	// DWT
		Index1=0;
		//Index2=2*nSupp-1;
		Index2=1;

		// 进行卷积，其中s1为低频部分，s2为高频部分的结果
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

			// 将结果存放在临时内存中
			ptemp[i] = s1/s;
			ptemp[i+CurN/2] = s2/s;

			//Index1 -= 2*nSupp;
			//Index2 += 2*nSupp;
			//Index1 += 2;
			Index2 += 4;
		}
	}

	// 否则进行IDWT
	else
	{	// IDWT
		//Index1 = CurN/2;
		Index1 = 0;
		Index2 = 0;
		
		// 进行卷积，其中其中s1为低频部分，s2为高频部分的结果
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
	
	// 将结果存入源图象中
	for (int i=0; i<CurN; i++)
		pDbSrc[i*nStep] = ptemp[i];

	// 释放临时内存，并返回
	delete[] ptemp;
	return TRUE;
}