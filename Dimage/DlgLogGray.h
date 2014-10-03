#pragma once


// DlgLogGray 对话框

class DlgLogGray : public CDialogEx
{
	DECLARE_DYNAMIC(DlgLogGray)

public:
	DlgLogGray(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgLogGray();

// 对话框数据
	enum { IDD = IDD_LogGray};
	
	double C;
	double *hist1;
	double *hist2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnChangeC();
};
