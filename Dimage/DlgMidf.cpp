// DlgMidf.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgMidf.h"
#include "afxdialogex.h"
#include "CImg.h"


// DlgMidf �Ի���

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


// DlgMidf ��Ϣ�������


void DlgMidf::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CWnd* pWnd = GetDlgItem(IDC_pic);
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	m_adp->Draw(pDC);

}
