#pragma once


// DlgHist 对话框

class DlgHist : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHist)

public:
	DlgHist(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgHist();

// 对话框数据
	enum { IDD = IDD_Hist };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double *m_pdHist;
	CSliderCtrl holdd;
	int holds;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnNMCustomdrawholdd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
