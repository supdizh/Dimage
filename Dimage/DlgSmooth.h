#pragma once


// DlgSmooth �Ի���

class DlgSmooth : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSmooth)

public:
	DlgSmooth(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgSmooth();

// �Ի�������
	enum { IDD = IDD_Smooth };
public:
	double * m_fpArray;//�Զ���ģ�����
	double m_fTempC;
	int m_intType;
	// ģ��߶�
	int	m_iTempH;
	// ģ����
	int	m_iTempW;
	// ģ������Ԫ��X����
	int	m_iTempMX;
	// ģ������Ԫ��Y����
	int	m_iTempMY;

	void UpdateEdit();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRad1();
	afx_msg void OnRad2();
	afx_msg void OnRad3();
	virtual void OnOK();
};
