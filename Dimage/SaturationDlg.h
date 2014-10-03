#pragma once
#include "DimageView.h"

// SaturationDlg 对话框

class SaturationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SaturationDlg)

public:
	SaturationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SaturationDlg();

// 对话框数据
	enum { IDD = IDD_SATURATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CSliderCtrl saturation;
	int satu;
	CDimageView *pView;  //通过指针控制pView，注意前面包含Viewclass.h,viewclass也要包含doc类
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
//	void DoOperation(void);
};