#pragma once


// DlgHist �Ի���

class DlgHist : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHist)

public:
	DlgHist(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgHist();

// �Ի�������
	enum { IDD = IDD_Hist };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
