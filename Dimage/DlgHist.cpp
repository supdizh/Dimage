// DlgHist.cpp : 实现文件
//

#include "stdafx.h"
#include "Dimage.h"
#include "DlgHist.h"
#include "afxdialogex.h"


// DlgHist 对话框

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


// DlgHist 消息处理程序


void DlgHist::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	// 字符串
	CString str;

	// 获取绘制坐标的控件
	CWnd* pWnd = GetDlgItem(IDC_Hist);

	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,640,250); //左上右下坐标
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// 创建画笔对象
	CPen* pPenGreen = new CPen;
	
	// 绿色画笔
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,240);
	
	// 水平轴
	pDC->LineTo(620,240);
	//pDC->LineTo(310,10);   //左上为00，下和右为正方向

	// 写X轴刻度值
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

	// 绘制X轴刻度
	for (i = 0; i < 256*2; i += 10)
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
	pDC->MoveTo(610,235);
	pDC->LineTo(620,240);
	pDC->LineTo(610,245);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	double maxHist = 0;

	for (i = 1; i < 255; i++)
	{
		maxHist = max(maxHist, m_pdHist[i]);//后面参数选最大放入maxHist  不算纯白，纯黑，保证高度
	}

	pDC->SelectObject(pPenBlue);

	for (i = 0; i < 256; i++)
	{
		pDC->MoveTo(2*i + 10, 240);
		pDC->LineTo(2*i + 10, int(240 - m_pdHist[i] / maxHist * 240)); //归一化
	};
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}

BOOL DlgHist::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	holdd.SetRange(0,255,0);
	holdd.SetPos(200);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgHist::OnNMCustomdrawholdd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	holds=holdd.GetPos();
	UpdateData(0);
}

void DlgHist::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	DestroyWindow();
}