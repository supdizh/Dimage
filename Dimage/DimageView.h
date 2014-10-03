
// DimageView.h : CDimageView 类的接口
//

#pragma once
#include "DimageDoc.h"

extern double Template_VSobel[];
extern double Template_HSobel[];
extern double Template_Laplace[];


class CDimageView : public CView
{
protected: // 仅从序列化创建
	CDimageView();
	DECLARE_DYNCREATE(CDimageView)

// 特性
public:
	CDimageDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CDimageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGeotran1();
	CImg* Move(int x, int y, bool ifresize, CImg* pImg);
	CImg* Rotate(int theta, bool ifresize, CImg* pImg);
	CImg* Scale(int x, int y, CImg* pImg);
	CImg* PosTran(CImg* pImg);
	CImg* Mirror(CImg* pImg,bool ifH);
	CImg* togray(CImg* pImg);
	CImg* LinearGray(double fA,double fB, CImg* pImg);
	CImg* LogGray(double C, CImg* pImg);
	CImg* Gamma(double gamma, double comp, CImg* pImg);
	CImg* Holda(int hold,CImg* pImg);
	CImg* Histeq(CImg* pImg);
	CImg* Inverse(CImg* pImg);
	void Saturation(int satu);
	CImg* Template(int nTempH, int nTempW, int nTempMY, int nTempMX,double *pfArray, double fCoef,CImg *pImg);
	CImg* Midf(CImg* pImg);
	CImg* adpMidf(CImg* pImg);
	CImg* adpthreshold(CImg* pImg);
	int GetMedianValue(int * pAryGray, int nFilterLen);
	CImg* Sobel(CImg* pImg);
	CImg* RobertEdge(CImg* pImg);
	CImg* Laplace(CImg* pImg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPostran();
	afx_msg void OnHmirror();
	afx_msg void OnVmirror();
	afx_msg void OnEditUndo();
	afx_msg void On24bit2gray();
	afx_msg void OnGenhist();
	afx_msg void Onlineargray();
	afx_msg void OnLoggray();
	afx_msg void OnGammagray();
	afx_msg void OnHisteq();
	afx_msg void OnSaturation();
	afx_msg void OnInverse();
	afx_msg void OnenSmoothf();
	afx_msg void OnenMidf();
	afx_msg void OnenSobels();
	afx_msg void OnenLapaces();
	afx_msg void Onenhigh();
	afx_msg void OnRobertEdge();


	afx_msg void OnAutothre();
	CImg* Autothres(CImg* pImg);
	afx_msg void OnSobelEdge();
	CImg* SobelEdge(CImg* pImg);
	afx_msg void OnPrewittEdge();
	CImg* PrewittEdge(CImg* pImg,int index);
	afx_msg void OnLogEdge();
	CImg* LogEdge(CImg* pImg);
	afx_msg void OnCanny();
	CImg* Canny(CImg* pImg);
	CImg* Dwtstep(CImg* pImg, int Inv);
	int Curdpth;
	double* Dwttemp;
	void ClearDwt(){delete[] Dwttemp;Dwttemp = NULL;}
	
	afx_msg void OnFwdDwt();
	afx_msg void OnBwdDwt();
};

#ifndef _DEBUG  // DimageView.cpp 中的调试版本
inline CDimageDoc* CDimageView::GetDocument() const
   { return reinterpret_cast<CDimageDoc*>(m_pDocument); }
#endif

