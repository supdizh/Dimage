#pragma once


// DlgGamma 对话框

class DlgGamma : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGamma)

public:
	DlgGamma(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgGamma();

// 对话框数据
	enum { IDD = IDD_GammaGray };
	double comp;
	double gamma;
	double *hist1;
	double *hist2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnPaint();
	afx_msg void OnChangegamma();
	afx_msg void OnChangecomp();
};
