#pragma once


// DlgGeoTran �Ի���

class DlgGeoTran : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGeoTran)

public:
	DlgGeoTran(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgGeoTran();

// �Ի�������
	enum { IDD = IDD_GeoTran };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��X

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
