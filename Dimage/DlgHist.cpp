// DlgHist.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgHist.h"
#include "afxdialogex.h"


// DlgHist �Ի���

IMPLEMENT_DYNAMIC(DlgHist, CDialogEx)

DlgHist::DlgHist(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgHist::IDD, pParent)
{
	m_pdHist=NULL;
	holds=200;
}

DlgHist::~DlgHist()
{
}

void DlgHist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_holdd, holdd);
	DDX_Text(pDX, IDC_holds, holds);
	DDV_MinMaxInt(pDX, holds, 0, 255);

}


BEGIN_MESSAGE_MAP(DlgHist, CDialogEx)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_holdd, &DlgHist::OnNMCustomdrawholdd)
END_MESSAGE_MAP()


// DlgHist ��Ϣ�������


void DlgHist::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	// �ַ���
	CString str;

	// ��ȡ��������Ŀؼ�
	CWnd* pWnd = GetDlgItem(IDC_Hist);

	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,640,250); //������������
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// �������ʶ���
	CPen* pPenGreen = new CPen;
	
	// ��ɫ����
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,240);
	
	// ˮƽ��
	pDC->LineTo(620,240);
	//pDC->LineTo(310,10);   //����Ϊ00���º���Ϊ������

	// дX��̶�ֵ
	str.Format("0");
	pDC->TextOut(10, 243, str);
	str.Format("50");
	pDC->TextOut(50*2+10, 243, str);
	str.Format("100");
	pDC->TextOut(210, 243, str);
	str.Format("150");
	pDC->TextOut(310, 243, str);
	str.Format("200");
	pDC->TextOut(410, 243, str);
	str.Format("255");
	pDC->TextOut(520, 243, str);
	
	int i;

	// ����X��̶�
	for (i = 0; i < 256*2; i += 10)
	{
		if ((i & 1) == 0)
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 240);
			pDC->LineTo(i + 10, 234);
		}
		else
		{
			// 5�ı���
			pDC->MoveTo(i + 10, 240);
			pDC->LineTo(i + 10, 236);
		}
	}
	
	// ����X���ͷ
	pDC->MoveTo(610,235);
	pDC->LineTo(620,240);
	pDC->LineTo(610,245);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	double maxHist = 0;

	for (i = 1; i < 255; i++)
	{
		maxHist = max(maxHist, m_pdHist[i]);//�������ѡ������maxHist  ���㴿�ף����ڣ���֤�߶�
	}

	pDC->SelectObject(pPenBlue);

	for (i = 0; i < 256; i++)
	{
		pDC->MoveTo(2*i + 10, 240);
		pDC->LineTo(2*i + 10, int(240 - m_pdHist[i] / maxHist * 240)); //��һ��
	};
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}

BOOL DlgHist::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	holdd.SetRange(0,255,0);
	holdd.SetPos(200);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void DlgHist::OnNMCustomdrawholdd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	holds=holdd.GetPos();
	UpdateData(0);
}

void DlgHist::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
	DestroyWindow();
}