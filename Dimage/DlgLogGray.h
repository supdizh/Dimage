#pragma once


// DlgLogGray �Ի���

class DlgLogGray : public CDialogEx
{
	DECLARE_DYNAMIC(DlgLogGray)

public:
	DlgLogGray(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgLogGray();

// �Ի�������
	enum { IDD = IDD_LogGray};
	
	double C;
	double *hist1;
	double *hist2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnChangeC();
};
