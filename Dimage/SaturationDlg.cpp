// SaturationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dimage.h"
#include "SaturationDlg.h"
#include "afxdialogex.h"


// SaturationDlg �Ի���

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


// SaturationDlg ��Ϣ�������


void SaturationDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
	DestroyWindow();
}  


void SaturationDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pView->OnEditUndo();
	CDialogEx::OnCancel();
	DestroyWindow();
}


BOOL SaturationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	saturation.SetRange(0,300,0);
	saturation.SetPos(100);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void SaturationDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	satu=saturation.GetPos();
	UpdateData(0);
//	*pResult = 0;
	pView->Saturation(satu);     //����ǹؼ���ÿslide��CALL������Viewclass�µ���SAT

}

