// SaturationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Dimage.h"
#include "SaturationDlg.h"
#include "afxdialogex.h"


// SaturationDlg 对话框

IMPLEMENT_DYNAMIC(SaturationDlg, CDialogEx)

SaturationDlg::SaturationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SaturationDlg::IDD, pParent)
	, satu(100)
	, pView(NULL)
{

}

SaturationDlg::~SaturationDlg()
{
}

void SaturationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, saturation);
	DDX_Text(pDX, IDC_EDIT1, satu);
	DDV_MinMaxInt(pDX, satu, 0, 300);
}


BEGIN_MESSAGE_MAP(SaturationDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SaturationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SaturationDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &SaturationDlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// SaturationDlg 消息处理程序


void SaturationDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	DestroyWindow();
}  


void SaturationDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	pView->OnEditUndo();
	CDialogEx::OnCancel();
	DestroyWindow();
}


BOOL SaturationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	saturation.SetRange(0,300,0);
	saturation.SetPos(100);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void SaturationDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	satu=saturation.GetPos();
	UpdateData(0);
//	*pResult = 0;
	pView->Saturation(satu);     //这个是关键，每slide有CALL，就在Viewclass下调用SAT

}

