// DlgSmooth.cpp : 实现文件
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgSmooth.h"
#include "afxdialogex.h"


// DlgSmooth 对话框

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


// DlgSmooth 消息处理程序


void DlgSmooth::OnRad1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置模板元素
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
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
	
}


void DlgSmooth::OnRad2()
{
	// 3×3高斯模板
	m_fTempC  = (double) (1.0 / 16.0);
	m_intType = 1;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	
	// 设置模板元素
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 2.0;
	m_fpArray[2] = 1.0;

	m_fpArray[3] = 2.0;
	m_fpArray[4] = 4.0;
	m_fpArray[5] = 2.0;

	m_fpArray[6] = 1.0;
	m_fpArray[7] = 2.0;
	m_fpArray[8] = 1.0;
	
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
	
}


void DlgSmooth::OnRad3()
{
	// 自定义模板
	m_intType = 2;
	
	// 更新文本框状态
	UpdateEdit();
}


void DlgSmooth::OnOK()
{
	// 获取用户设置（更新）
	UpdateData(TRUE);
	// 更新模板元素数组（将有效元素放置在数组的前面）
	double C=0;
	for (int i = 0; i < m_iTempH*m_iTempW; i++)
	{			
		C =C + m_fpArray[i];
	}
	m_fTempC = 1/C;
	// 更新
	UpdateData(FALSE);
	CDialogEx::OnOK();
}

void DlgSmooth::UpdateEdit()
{
	BOOL bEnable;
	// 循环变量
	int		i;
	//int		j;
	
	// 判断是不是自定义模板
	if (m_intType == 2) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	//// 设置文本框可用状态 我没这些案件
	//(CEdit *) GetDlgItem(IDC_EDIT_TEMPH)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_TEMPW)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_TEMPC)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	//(CEdit *) GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);
	
	// IDC_EDIT_V0等ID其实是一个整数，它的数值定义在Resource.h中定义。
	
	// 设置模板元素文本框Enable状态
	for (i = IDC_V0; i <= IDC_V8; i++)
	{
		// 设置文本框不可编辑
		(CEdit *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	//// 显示应该可见的模板元素文本框,我没这段
	//for (i = 0; i < m_iTempH; i++)
	//{
	//	for (j = 0; j < m_iTempW; j++)
	//	{
	//		// 设置文本框可见
	//		(CEdit *) GetDlgItem(IDC_V0 + i*5 + j)->ShowWindow(SW_SHOW);
	//	}
	//}
	
	//// 隐藏应该不可见的模板元素文本框（前m_iTempH行的后几列）
	//for (i = 0; i < m_iTempH; i++)
	//{
	//	for (j = m_iTempW; j < 5; j++)
	//	{
	//		// 设置不可见
	//		(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
	//	}
	//}
	
	//// 隐藏应该不可见的模板元素文本框（后几行）
	//for (i = m_iTempH; i < 5; i++)
	//{
	//	for (j = 0; j < 5; j++)
	//	{
	//		// 设置不可见
	//		(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
	//	}
	//}

}