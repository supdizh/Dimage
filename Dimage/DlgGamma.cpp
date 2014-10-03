// DlgGamma.cpp : 实现文件
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgGamma.h"
#include "afxdialogex.h"
#include "math.h"


// DlgGamma 对话框

IMPLEMENT_DYNAMIC(DlgGamma, CDialogEx)

DlgGamma::DlgGamma(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgGamma::IDD, pParent)
{
	comp=0;
	gamma=1;
}

DlgGamma::~DlgGamma()
{
}

void DlgGamma::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_comp, comp);
	DDX_Text(pDX, IDC_gamma, gamma);
}


BEGIN_MESSAGE_MAP(DlgGamma, CDialogEx)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_gamma, &DlgGamma::OnChangegamma)
	ON_EN_KILLFOCUS(IDC_comp, &DlgGamma::OnChangecomp)
END_MESSAGE_MAP()


// DlgGamma 消息处理程序


void DlgGamma::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	// 字符串
	CString str;

	// 获取绘制坐标的控件
	CWnd* pWnd = GetDlgItem(IDC_hist4);

	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,320,250); //左上右下坐标
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,1.7,RGB(0,0, 255));
	
	// 创建画笔对象
	CPen* pPenGreen = new CPen;
	
	// 绿色画笔
	pPenGreen->CreatePen(PS_SOLID,1.7,RGB(0,255,0));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,240);
	
	// 水平轴
	pDC->LineTo(310,240);
	//pDC->LineTo(310,10);   //左上为00，下和右为正方向

	// 写X轴刻度值
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

	// 绘制X轴刻度
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 240);
			pDC->LineTo(i + 10, 234);
		}
		else
		{
			// 5的倍数
			pDC->MoveTo(i + 10, 240);
			pDC->LineTo(i + 10, 236);
		}
	}
	
	// 绘制X轴箭头
	pDC->MoveTo(305,235);
	pDC->LineTo(310,240);
	pDC->LineTo(305,245);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	double temp = 0;
	double maxHist = 0;
	int index=0;
	for (i = 0; i < 256; i++)
	{
		temp= pow(double(i),1/gamma)-comp;
		if(temp-1<0||temp-1>255)
			hist2[i]=0;
		else
			index =(int)(temp-1);
			hist2[i] = hist1[index];

		if(i!=0&&i!=255) maxHist = max(maxHist, hist1[i]);//后面参数选最大放入maxHist
	}


	pDC->SelectObject(pPenBlue);
	pDC->MoveTo(12,int(240-hist1[0]/maxHist * 240)<0?0:int(240 - hist1[i] / maxHist * 240));
	for (i = 1; i < 256; i++)
	{
		pDC->LineTo(i + 10, int(240 - hist1[i] / maxHist * 240)<0?0:int(240 - hist1[i] / maxHist * 240)); //归一化
		pDC->MoveTo(i + 10, int(240 - hist1[i] / maxHist * 240)<0?0:int(240 - hist1[i] / maxHist * 240));
	};

	pDC->SelectObject(pPenGreen);
	pDC->MoveTo(12,int(240-hist2[0]/maxHist * 240)<0?0:int(240 - hist2[i] / maxHist * 240));

	for (i = 1; i < 256; i++)
	{
		pDC->LineTo(i + 10, int(240 - hist2[i] / maxHist * 240)<0?0:int(240 - hist2[i] / maxHist * 240)); //归一化
		pDC->MoveTo(i + 10, int(240 - hist2[i] / maxHist * 240)<0?0:int(240 - hist2[i] / maxHist * 240));
	};
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}


void DlgGamma::OnChangegamma()
{
	UpdateData(TRUE);
	// 重绘
	Invalidate(1);
}


void DlgGamma::OnChangecomp()
{
	UpdateData(TRUE);
	// 重绘
	Invalidate(1);
}
