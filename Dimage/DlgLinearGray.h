#pragma once


// DlgLinearGray �Ի���

class DlgLinearGray : public CDialogEx
{
	DECLARE_DYNAMIC(DlgLinearGray)

public:
	DlgLinearGray(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgLinearGray();

// �Ի�������
	enum { IDD = IDD_LinearGray };
	
	double m_fA;
	double m_fB;
	double *hist1;
	double *hist2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnChangefA();
	afx_msg void OnchangefB();
};
