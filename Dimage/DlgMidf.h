#pragma once

#include "CImg.h"
// DlgMidf �Ի���

class DlgMidf : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMidf)

public:
	DlgMidf(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgMidf();

// �Ի�������
	enum { IDD = IDD_Midf };
public:
	CImg* m_adp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
