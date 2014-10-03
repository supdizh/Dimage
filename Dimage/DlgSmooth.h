#pragma once


// DlgSmooth 对话框

class DlgSmooth : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSmooth)

public:
	DlgSmooth(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgSmooth();

// 对话框数据
	enum { IDD = IDD_Smooth };
public:
	double * m_fpArray;//自定义模板参数
	double m_fTempC;
	int m_intType;
	// 模板高度
	int	m_iTempH;
	// 模板宽度
	int	m_iTempW;
	// 模板中心元素X坐标
	int	m_iTempMX;
	// 模板中心元素Y坐标
	int	m_iTempMY;

	void UpdateEdit();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRad1();
	afx_msg void OnRad2();
	afx_msg void OnRad3();
	virtual void OnOK();
};
