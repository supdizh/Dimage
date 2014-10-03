#pragma once


// DlgLinearGray 对话框

class DlgLinearGray : public CDialogEx
{
	DECLARE_DYNAMIC(DlgLinearGray)

public:
	DlgLinearGray(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgLinearGray();

// 对话框数据
	enum { IDD = IDD_LinearGray };
	
	double m_fA;
	double m_fB;
	double *hist1;
	double *hist2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnChangefA();
	afx_msg void OnchangefB();
};
