// DlgGeoTran.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgGeoTran.h"
#include "afxdialogex.h"


// DlgGeoTran �Ի���

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


// DlgGeoTran ��Ϣ�������


void DlgGeoTran::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);//1�ǰѿؼ���ֵ����CLASS������0�ǰ�CLASS���������ؼ�
	resize=ifresize.GetCheck();    
	CDialogEx::OnOK();
}
