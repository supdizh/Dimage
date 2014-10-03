// DlgSmooth.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgSmooth.h"
#include "afxdialogex.h"


// DlgSmooth �Ի���

IMPLEMENT_DYNAMIC(DlgSmooth, CDialogEx)

DlgSmooth::DlgSmooth(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgSmooth::IDD, pParent)
{
	m_intType = -1;
	m_iTempH = 0;
	m_iTempW = 0;
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_fTempC = 0.0;
}

DlgSmooth::~DlgSmooth()
{
}

void DlgSmooth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RAD1, m_intType);
	DDX_Text(pDX, IDC_V0, m_fpArray[0]);
	DDX_Text(pDX, IDC_V1, m_fpArray[1]);
	DDX_Text(pDX, IDC_V2, m_fpArray[2]);
	DDX_Text(pDX, IDC_V3, m_fpArray[3]);
	DDX_Text(pDX, IDC_V4, m_fpArray[4]);
	DDX_Text(pDX, IDC_V5, m_fpArray[5]);
	DDX_Text(pDX, IDC_V6, m_fpArray[6]);
	DDX_Text(pDX, IDC_V7, m_fpArray[7]);
	DDX_Text(pDX, IDC_V8, m_fpArray[8]);
}


BEGIN_MESSAGE_MAP(DlgSmooth, CDialogEx)
	ON_BN_CLICKED(IDC_RAD1, &DlgSmooth::OnRad1)
	ON_BN_CLICKED(IDC_RAD2, &DlgSmooth::OnRad2)
	ON_BN_CLICKED(IDC_RAD3, &DlgSmooth::OnRad3)
END_MESSAGE_MAP()


// DlgSmooth ��Ϣ�������


void DlgSmooth::OnRad1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ����ģ��Ԫ��
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 1.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 1.0;
	m_fpArray[4] = 1.0;
	m_fpArray[5] = 1.0;
	m_fpArray[6] = 1.0;
	m_fpArray[7] = 1.0;
	m_fpArray[8] = 1.0;

	m_fTempC  = (double) (1.0 / 9.0);
	m_intType = 0;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
	
}


void DlgSmooth::OnRad2()
{
	// 3��3��˹ģ��
	m_fTempC  = (double) (1.0 / 16.0);
	m_intType = 1;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	
	// ����ģ��Ԫ��
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 2.0;
	m_fpArray[2] = 1.0;

	m_fpArray[3] = 2.0;
	m_fpArray[4] = 4.0;
	m_fpArray[5] = 2.0;

	m_fpArray[6] = 1.0;
	m_fpArray[7] = 2.0;
	m_fpArray[8] = 1.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
	
}


void DlgSmooth::OnRad3()
{
	// �Զ���ģ��
	m_intType = 2;
	
	// �����ı���״̬
	UpdateEdit();
}


void DlgSmooth::OnOK()
{
	// ��ȡ�û����ã����£�
	UpdateData(TRUE);
	// ����ģ��Ԫ�����飨����ЧԪ�ط����������ǰ�棩
	double C=0;
	for (int i = 0; i < m_iTempH*m_iTempW; i++)
	{			
		C =C + m_fpArray[i];
	}
	m_fTempC = 1/C;
	// ����
	UpdateData(FALSE);
	CDialogEx::OnOK();
}

void DlgSmooth::UpdateEdit()
{
	BOOL bEnable;
	// ѭ������
	int		i;
	//int		j;
	
	// �ж��ǲ����Զ���ģ��
	if (m_intType == 2) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	//// �����ı������״̬ ��û��Щ����
	//(CEdit *) GetDlgItem(IDC_EDIT_TEMPH)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_TEMPW)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_TEMPC)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);
	
	// IDC_EDIT_V0��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
	
	// ����ģ��Ԫ���ı���Enable״̬
	for (i = IDC_V0; i <= IDC_V8; i++)
	{
		// �����ı��򲻿ɱ༭
		(CEdit *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	//// ��ʾӦ�ÿɼ���ģ��Ԫ���ı���,��û���
	//for (i = 0; i < m_iTempH; i++)
	//{
	//	for (j = 0; j < m_iTempW; j++)
	//	{
	//		// �����ı���ɼ�
	//		(CEdit *) GetDlgItem(IDC_V0 + i*5 + j)->ShowWindow(SW_SHOW);
	//	}
	//}
	
	//// ����Ӧ�ò��ɼ���ģ��Ԫ���ı���ǰm_iTempH�еĺ��У�
	//for (i = 0; i < m_iTempH; i++)
	//{
	//	for (j = m_iTempW; j < 5; j++)
	//	{
	//		// ���ò��ɼ�
	//		(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
	//	}
	//}
	
	//// ����Ӧ�ò��ɼ���ģ��Ԫ���ı��򣨺��У�
	//for (i = m_iTempH; i < 5; i++)
	//{
	//	for (j = 0; j < 5; j++)
	//	{
	//		// ���ò��ɼ�
	//		(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
	//	}
	//}

}