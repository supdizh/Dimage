// DlgGeoTran.cpp : 实现文件
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgGeoTran.h"
#include "afxdialogex.h"


// DlgGeoTran 对话框

IMPLEMENT_DYNAMIC(DlgGeoTran, CDialogEx)

DlgGeoTran::DlgGeoTran(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgGeoTran::IDD, pParent)
{
	Xoffset=0;
	Yoffset=0;
	resize=false;
	theta=0;
	scalex=100;
	scaley=100;
}

DlgGeoTran::~DlgGeoTran()
{
}

void DlgGeoTran::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Xoffset, Xoffset);
	DDX_Text(pDX, IDC_Yoffset, Yoffset);
	DDX_Text(pDX, IDC_theta, theta);
	DDX_Control(pDX, IDC_ifresize, ifresize);
	DDX_Text(pDX, IDC_Scalex, scalex);
	DDX_Text(pDX, IDC_Scaley, scaley);

}


BEGIN_MESSAGE_MAP(DlgGeoTran, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgGeoTran::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgGeoTran 消息处理程序


void DlgGeoTran::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);//1是把控件的值交给CLASS变量，0是把CLASS变量交给控件
	resize=ifresize.GetCheck();    
	CDialogEx::OnOK();
}
