#pragma once


// DlgGamma �Ի���

class DlgGamma : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGamma)

public:
	DlgGamma(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgGamma();

// �Ի�������
	enum { IDD = IDD_GammaGray };
	double comp;
	double gamma;
	double *hist1;
	double *hist2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnPaint();
	afx_msg void OnChangegamma();
	afx_msg void OnChangecomp();
};
