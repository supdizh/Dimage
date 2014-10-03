#pragma once


// DlgGeoTran 对话框

class DlgGeoTran : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGeoTran)

public:
	DlgGeoTran(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgGeoTran();

// 对话框数据
	enum { IDD = IDD_GeoTran };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持X

	DECLARE_MESSAGE_MAP()
public:
	int Xoffset;
	int Yoffset;
	CButton ifresize;
	bool resize;
	double theta;
	int scalex;
	int scaley;


	afx_msg void OnBnClickedOk();
};
