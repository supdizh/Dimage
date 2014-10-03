// DlgMidf.cpp : 实现文件
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgMidf.h"
#include "afxdialogex.h"
#include "CImg.h"


// DlgMidf 对话框

IMPLEMENT_DYNAMIC(DlgMidf, CDialogEx)

DlgMidf::DlgMidf(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgMidf::IDD, pParent)
{
	m_adp = NULL;
}

DlgMidf::~DlgMidf()
{
}

void DlgMidf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMidf, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DlgMidf 消息处理程序


void DlgMidf::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CWnd* pWnd = GetDlgItem(IDC_pic);
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	m_adp->Draw(pDC);

}
