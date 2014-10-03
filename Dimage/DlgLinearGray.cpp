// DlgLinearGray.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgLinearGray.h"
#include "afxdialogex.h"


// DlgLinearGray �Ի���

IMPLEMENT_DYNAMIC(DlgLinearGray, CDialogEx)

DlgLinearGray::DlgLinearGray(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgLinearGray::IDD, pParent)
{
	m_fA=0;
	m_fB=0;
}

DlgLinearGray::~DlgLinearGray()
{
}

void DlgLinearGray::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_fA, m_fA);
	DDX_Text(pDX, IDC_fB, m_fB);
}


BEGIN_MESSAGE_MAP(DlgLinearGray, CDialogEx)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_fA, &DlgLinearGray::OnChangefA)
	ON_EN_KILLFOCUS(IDC_fB, &DlgLinearGray::OnchangefB)
END_MESSAGE_MAP()


// DlgLinearGray ��Ϣ�������


void DlgLinearGray::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	// �ַ���
	CString str;

	// ��ȡ��������Ŀؼ�
	CWnd* pWnd = GetDlgItem(IDC_hist1);

	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,320,250); //������������
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,1.7,RGB(0,0, 255));
	
	// �������ʶ���
	CPen* pPenGreen = new CPen;
	
	// ��ɫ����
	pPenGreen->CreatePen(PS_SOLID,1.7,RGB(0,255,0));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,240);
	
	// ˮƽ��
	pDC->LineTo(310,240);
	//pDC->LineTo(310,10);   //����Ϊ00���º���Ϊ������

	// дX��̶�ֵ
	str.Format("0");
	pDC->TextOut(10, 243, str);
	str.Format("50");
	pDC->TextOut(50+10, 243, str);
	str.Format("100");
	pDC->TextOut(110, 243, str);
	str.Format("150");
	pDC->TextOut(160, 243, str);
	str.Format("200");
	pDC->TextOut(210, 243, str);
	str.Format("255");
	pDC->TextOut(265, 243, str);
	
	int i;

	// ����X��̶�
	for (i = 0; i < 256; i += 5)
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
	pDC->MoveTo(305,235);
	pDC->LineTo(310,240);
	pDC->LineTo(305,245);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	double maxHist = 0;
	int index=0;
	for (i = 0; i < 256; i++)
	{
		if((i-m_fB)/m_fA<0||(i-m_fB)/m_fA>255)
			hist2[i]=0;
		else
			index =(int)(i-m_fB)/m_fA;
			hist2[i] = hist1[index];

		if(i!=0&&i!=255) maxHist = max(maxHist, hist1[i]);//�������ѡ������maxHist
	}


	pDC->SelectObject(pPenBlue);
	pDC->MoveTo(12,int(240-hist1[0]/maxHist * 240)<0?0:int(240 - hist1[i] / maxHist * 240));
	for (i = 1; i < 256; i++)
	{
		pDC->LineTo(i + 10, int(240 - hist1[i] / maxHist * 240)<0?0:int(240 - hist1[i] / maxHist * 240)); //��һ��
		pDC->MoveTo(i + 10, int(240 - hist1[i] / maxHist * 240)<0?0:int(240 - hist1[i] / maxHist * 240));
	};

	pDC->SelectObject(pPenGreen);
	pDC->MoveTo(12,int(240-hist2[0]/maxHist * 240)<0?0:int(240 - hist2[i] / maxHist * 240));

	for (i = 1; i < 256; i++)
	{
		pDC->LineTo(i + 10, int(240 - hist2[i] / maxHist * 240)<0?0:int(240 - hist2[i] / maxHist * 240)); //��һ��
		pDC->MoveTo(i + 10, int(240 - hist2[i] / maxHist * 240)<0?0:int(240 - hist2[i] / maxHist * 240));
	};
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}


void DlgLinearGray::OnChangefA()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	// �ػ�
	Invalidate(1);
}


void DlgLinearGray::OnchangefB()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	// �ػ�
	Invalidate(1);
}
