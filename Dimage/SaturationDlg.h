#pragma once
#include "DimageView.h"

// SaturationDlg �Ի���

class SaturationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SaturationDlg)

public:
	SaturationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SaturationDlg();

// �Ի�������
	enum { IDD = IDD_SATURATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CSliderCtrl saturation;
	int satu;
	CDimageView *pView;  //ͨ��ָ�����pView��ע��ǰ�����Viewclass.h,viewclassҲҪ����doc��
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
//	void DoOperation(void);
};