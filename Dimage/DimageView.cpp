
// DimageView.cpp : CDimageView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Dimage.h"
#endif


#include<cmath>
#include "DimageDoc.h"
#include "DimageView.h"
#include "CImg.h"
#include "DlgGeoTran.h"
#include "DlgHist.h"
#include "DlgLinearGray.h"
#include "DlgLogGray.h"
#include "DlgGamma.h"
#include "SaturationDlg.h"
#include "DlgSmooth.h"
#include "DlgMidf.h"

#define PI 3.14

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//from DWT.cpp
int Log2(int n);
BOOL DWTStep_2D(double* pDbSrc, int nCurWLevel, int nCurHLevel,int nMaxWLevel, int nMaxHLevel, int Inv);


double Template_Laplace[9] = {-1, -1, -1,
-1, 8, -1,
-1, -1, -1};
// Sobel��ֱ��Ե���
double Template_HSobel[9]={-1, 0, 1, 
-2, 0, 2, 
-1 ,0 , 1};

// Sobelˮƽ��Ե���
double Template_VSobel[9]={-1, -2, -1,
0, 0, 0, 
1, 2, 1};
// CDimageView


IMPLEMENT_DYNCREATE(CDimageView, CView)

BEGIN_MESSAGE_MAP(CDimageView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_GeoTran1, &CDimageView::OnGeotran1)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_PosTran, &CDimageView::OnPostran)
	ON_COMMAND(ID_Hmirror, &CDimageView::OnHmirror)
	ON_COMMAND(ID_Vmirror, &CDimageView::OnVmirror)
	ON_COMMAND(ID_EDIT_UNDO, &CDimageView::OnEditUndo)
	ON_COMMAND(ID_24bit2gray, &CDimageView::On24bit2gray)
	ON_COMMAND(ID_Genhist, &CDimageView::OnGenhist)
	ON_COMMAND(ID_lineargray, &CDimageView::Onlineargray)
	ON_COMMAND(ID_LogGray, &CDimageView::OnLoggray)
	ON_COMMAND(ID_GammaGray, &CDimageView::OnGammagray)
	ON_COMMAND(ID_Histeq, &CDimageView::OnHisteq)
	ON_COMMAND(ID_SATURATION, &CDimageView::OnSaturation)
	ON_COMMAND(ID_Inverse, &CDimageView::OnInverse)
	ON_COMMAND(ID_en_Smoothf, &CDimageView::OnenSmoothf)
	ON_COMMAND(ID_en_Midf, &CDimageView::OnenMidf)
	ON_COMMAND(ID_en_Sobels, &CDimageView::OnenSobels)
	ON_COMMAND(ID_en_Lapaces, &CDimageView::OnenLapaces)
	ON_COMMAND(ID_en_high, &CDimageView::Onenhigh)
	ON_COMMAND(ID_32811, &CDimageView::OnRobertEdge)
	ON_COMMAND(ID_32816, &CDimageView::OnAutothre)
	ON_COMMAND(ID_32812, &CDimageView::OnSobelEdge)
	ON_COMMAND(ID_32813, &CDimageView::OnPrewittEdge)
	ON_COMMAND(ID_32814, &CDimageView::OnLogEdge)
	ON_COMMAND(ID_32815, &CDimageView::OnCanny)
	//ON_COMMAND(ID_32817, &CDimageView::OnFwdDwt)
	//ON_COMMAND(ID_32818, &CDimageView::OnBkwDwt)
	ON_COMMAND(ID_32819, &CDimageView::OnFwdDwt)
	ON_COMMAND(ID_32820, &CDimageView::OnBwdDwt)
END_MESSAGE_MAP()

// CDimageView ����/����

CDimageView::CDimageView()
	: Curdpth(0)
	, Dwttemp(NULL)
{
	// TODO: �ڴ˴���ӹ������
	TRACE("View constructed");
}

CDimageView::~CDimageView()
{
}

BOOL CDimageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ	

	return CView::PreCreateWindow(cs);
}

// CDimageView ����

void CDimageView::OnDraw(CDC* pDC)
{
	CDimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	pDoc->m_Img->Draw(pDC);

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CDimageView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDimageView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDimageView ���

#ifdef _DEBUG
void CDimageView::AssertValid() const
{
	CView::AssertValid();
}

void CDimageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDimageDoc* CDimageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDimageDoc)));
	return (CDimageDoc*)m_pDocument;
}
#endif //_DEBUG


// CDimageView ��Ϣ�������


void CDimageView::OnGeotran1()
{
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	DlgGeoTran dlg; //�����Ի���IDOK
	dlg.DoModal();

	CImg* Imgtemp = NULL; //���ֱ仯�м����
	Imgtemp = Move(dlg.Xoffset,dlg.Yoffset,dlg.resize,pDoc->m_OImg); //��ָ�봫�����ݵ�ַ����Σ�� 

	delete []pDoc->m_Img; //��ֵǰ��ɾ	
	pDoc->m_Img = Rotate(dlg.theta,dlg.resize, Imgtemp);

	delete []Imgtemp;
	Imgtemp = Scale(dlg.scalex,dlg.scaley,pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img = Imgtemp;
	Imgtemp = NULL;
	
	Invalidate(1); // Bool Erase =1���Ȳ����ڻ���ˢ��Ч����=0��������

}


CImg* CDimageView::Move(int x, int y ,bool ifresize, CImg* pImg)
{
	CImg* newImg = new CImg(*pImg);
	//�ı���ͼ���С
	int old_width,old_height,width,height;
	width=old_width=newImg->GetWidthPixel();
	height=old_height=newImg->GetHeight();

	//if(!ifunchange)
	//{
	//	width=old_width+x;
	//	height=old_height+y;
	//	if((width<=0)||(height<=0))
	//	{
	//		CString test("��ѽ��ͼ��������û�ˣ�");
	//		CString title("��ֵ����");
	//		MessageBox(test,title,0);
	//		return NULL;
	//	}
	//}

	if(ifresize)
	{	
		width=old_width+x;
		height=old_height+y;
		newImg->ImResize(height,width);
	}


	int i0,j0,i,j;
	COLORREF color;

	
	for(i=0;i<width;i++)
		for(j=0;j<height;j++)
		{
			i0=i-x;j0=j-y;
			if((i0>=0)&&(i0<old_width)&&(j0>=0)&&(j0<old_height))
			{
				color=pImg->GetPixel(i0,j0);
			}
			else
			{	
				color=RGB(255,255,255);
				//if(!ifunchange)
				//	color=RGB(255,255,255);
				//else
				//{
				//	if(!ifloop)
				//		color=RGB(255,255,255);
				//	else
				//	{
				//		i0=i0%width;
				//		j0=j0%height;
				//		if(i0<0)
				//			i0+=width;
				//		if(i0>=width)
				//			i0-=width;
				//		if(j0<0)
				//			j0+=height;
			 // 			if(j0>=height)
				//			j0-=height;
				//		color=pImg->GetPixel(i0,j0);
				//	}
				//}
			}
			newImg->SetPixel(i,j,color);
		}
	return newImg;
}


BOOL CDimageView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rc; 
	GetClientRect(&rc);  
	pDC->FillSolidRect(&rc ,  RGB(200,200,200)  ); 
	//return __super::OnEraseBkgnd(pDC);
	return TRUE;
}

CImg* CDimageView::Rotate(int theta, bool ifrisize, CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	//�ı���ͼ���С
	int old_width,old_height,width,height;
	float t=(float)theta/180*PI;
	width=old_width=mImg->GetWidthPixel();
	height=old_height=mImg->GetHeight();

	if (ifrisize){
		double alfa= atan((double)old_height/old_width); //�öԽ�����������ͼ�εı߽�
		double l= sqrt((double)(old_width*old_width+old_height*old_height)); //�Խ��߳���

		float h1=abs(sin(t+alfa)), h2=abs(sin(t-alfa));  //�����Խ�����Y����ͶӰ
		float w1=abs(cos(t+alfa)), w2=abs(cos(t-alfa));	//X����
		if(h1>h2)
			height= (int)(l*h1);
		else
			height= (int)(l*h2);   //ȡ����ͶӰֵ�γɱ߽�
		if(w1>w2)  
			width= (int)(l*w1);
		else
			width= (int)(l*w2);
	}

	int x,y,x0,y0,i0,j0,i,j;
	COLORREF color;

	mImg->ImResize(height,width);
	for(i=0;i<width;i++)
		for(j=0;j<height;j++)
		{
			x=i-width/2;
			y=j-height/2;
			x0=x*cos(t)+y*sin(t);
			y0=y*cos(t)-x*sin(t);
			i0=x0+old_width/2;
			j0=y0+old_height/2;
			if((i0>=old_width)||(i0<0)||(j0>=old_height)||(j0<0))
				color=RGB(255,255,255);
			else
				color=pImg->GetPixel(i0,j0);
			mImg->SetPixel(i,j,color);
		}
	return mImg;
}

CImg* CDimageView::Scale(int x, int y, CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j,i0,j0;
	width=abs(x*mImg->GetWidthPixel()/100); //�����ٷֱ�
	height=abs(y*mImg->GetHeight()/100);
	mImg->ImResize(height,width);
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			if(x>0)
				i0=i*100/x;
			else
				i0=(i-width+1)*100/x;
			if(y>0)
				j0=j*100/y;
			else
				j0=(j-height+1)*100/y;      //ȫ�������ٽ���ֵ double->int�Զ�����

			mImg->SetPixel(i,j,pImg->GetPixel(i0,j0));
		}
	}
	return mImg;
}

CImg* CDimageView::PosTran(CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	width=mImg->GetHeight();
	height=mImg->GetWidthPixel();
	mImg->ImResize(height,width);
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			mImg->SetPixel(i,j,pImg->GetPixel(j,i));
		}
	}
	return mImg;
}
void CDimageView::OnPostran()
{
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img = PosTran(pDoc->m_OImg);
	Invalidate(1);
}



void CDimageView::OnHmirror()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img = Mirror(pDoc->m_OImg,0);
	Invalidate(1);
}


void CDimageView::OnVmirror()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img = Mirror(pDoc->m_OImg,1);
	Invalidate(1);
}

CImg* CDimageView::Mirror(CImg* pImg,bool ifV)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
   
	for(i=0;i<width;i++)
	{
		
		for(j=0;j<height;j++)
		{
			if (ifV)
				mImg->SetPixel(i,j,pImg->GetPixel(i,height-j-1));
			else
				mImg->SetPixel(i,j,pImg->GetPixel(width-i-1,j));
		}//for j
	}//for i
	return mImg;
}


void CDimageView::OnEditUndo()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_OImg->IsValidate()) //ImgΪNULL���޷�����
		return;


	delete []pDoc->m_Img;
	pDoc->m_Img= new CImg(*pDoc->m_OImg);
	Invalidate(1);
	

}


void CDimageView::On24bit2gray()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img = togray(pDoc->m_OImg);
	Invalidate(1);
}

CImg* CDimageView::togray(CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	COLORREF ref;
	BYTE r, g, b, gray;
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
   
	for(i=0;i<width;i++)
	{
		
		for(j=0;j<height;j++)
		{
			ref = pImg->GetPixel(i , j);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);
			gray = b * 0.114 + g * 0.587 + r * 0.299 + 0.5;
			mImg->SetPixel(i,j,RGB(gray,gray,gray));

		}//for j
	}//for i
	return mImg;
}

void CDimageView::OnGenhist()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;
	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	int hold;
	double hist[256];//ֱ��ͼ����
	//double pdHistr[256],pdHistg[256],pdHistb[256];
	
	pDoc->m_Img->GenHist(hist); //histһά����ͷָ��
	//pDoc->m_Img->GenHistrgb(pdHistr,pdHistg,pdHistb); //histһά����ͷָ��
	
	
	DlgHist dlg; //�����Ի���IDOK
	//dlg.m_pdHist  =pdHistb;
	dlg.holds=200;
	dlg.m_pdHist  = hist;

	if (dlg.DoModal() != IDOK){
		return;
	}
	hold=dlg.holds;

	delete []pDoc->m_Img;
	pDoc->m_Img = Holda(hold,pDoc->m_OImg);
	Invalidate(1);
}

CImg* CDimageView::Holda(int hold,CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	COLORREF ref;
	BYTE r, g, b, gray;
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
   
	for(i=0;i<width;i++)
	{
		
		for(j=0;j<height;j++)
		{
			ref = pImg->GetPixel(i , j);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);
			gray = b * 0.114 + g * 0.587 + r * 0.299 + 0.5;
			gray = gray>hold?255:0;
			mImg->SetPixel(i,j,RGB(gray,gray,gray));

		}//for j
	}//for i
	return mImg;
}

void CDimageView::Onlineargray()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);


	DlgLinearGray dlg;
	
	// ���Ա任��б��
	double fA;
	// ���Ա任�Ľؾ�
	double fB;
	// ��ʼ������ֵ
	dlg.m_fA = 2;
	dlg.m_fB = -20;
	double hist[256];
	double histo[256];
	pDoc->m_Img->GenHist(hist); //histһά����ͷָ��
	dlg.hist1 = hist;
	dlg.hist2 = histo;
	// ��ʾ�Ի�����ʾ�û��趨����
	dlg.DoModal();
	fA = dlg.m_fA;
	fB = dlg.m_fB;

	
	delete []pDoc->m_Img;
	pDoc->m_Img = LinearGray(fA,fB,pDoc->m_OImg);
	Invalidate(1);
}


CImg* CDimageView::LinearGray(double fA,double fB, CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	COLORREF ref;
	BYTE r, g, b;
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
   
	for(i=0;i<width;i++)
	{
		
		for(j=0;j<height;j++)
		{
			ref = pImg->GetPixel(i , j);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);

			if (fA*r+fB<0)
				r=0;
			else if (fA*r+fB>255)
				r=255;
			else
				r=fA*r+fB;

			if (fA*g+fB<0)
				g=0;
			else if (fA*g+fB>255)
				g=255;
			else
				g=fA*g+fB;

			if (fA*b+fB<0)
				b=0;
			else if (fA*b+fB>255)
				b=255;
			else
				b=fA*b+fB;

			mImg->SetPixel(i,j,RGB(r,g,b));

		}//for j
	}//for i
	return mImg;
}

void CDimageView::OnLoggray()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);


	DlgLogGray dlg;
	
	double C;
	dlg.C = 10;
	double hist[256];
	double histo[256];
	pDoc->m_Img->GenHist(hist); //histһά����ͷָ��
	dlg.hist1 = hist;
	dlg.hist2 = histo;
	// ��ʾ�Ի�����ʾ�û��趨����
	dlg.DoModal();
	C=dlg.C;

	delete []pDoc->m_Img;
	pDoc->m_Img = LogGray(C,pDoc->m_OImg);
	Invalidate(1);
}

CImg* CDimageView::LogGray(double C, CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	COLORREF ref;
	BYTE r, g, b;
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
   
	for(i=0;i<width;i++)
	{
		
		for(j=0;j<height;j++)
		{
			ref = pImg->GetPixel(i , j);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);

			if (C*log(1+(double)r)<0)
				r=0;
			else if (C*log(1+(double)r)>255)
				r=255;
			else
				r=C*log(1+(double)r);

			if (C*log(1+(double)g)<0)
				g=0;
			else if (C*log(1+(double)g)>255)
				g=255;
			else
				g=C*log(1+(double)g);

			if (C*log(1+(double)b)<0)
				b=0;
			else if (C*log(1+(double)b)>255)
				b=255;
			else
				b=C*log(1+(double)b);

			mImg->SetPixel(i,j,RGB(r,g,b));

		}//for j
	}//for i
	return mImg;
}


void CDimageView::OnGammagray()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);


	DlgGamma dlg;
	
	double gamma;
	double comp;
	dlg.gamma = 1.3;
	dlg.comp = 2;
	double hist[256];
	double histo[256];
	pDoc->m_Img->GenHist(hist); //histһά����ͷָ��
	dlg.hist1 = hist;
	dlg.hist2 = histo;
	// ��ʾ�Ի�����ʾ�û��趨����
	dlg.DoModal();
	gamma=dlg.gamma;
	comp=dlg.comp;

	delete []pDoc->m_Img;
	pDoc->m_Img = Gamma(gamma,comp,pDoc->m_OImg);
	Invalidate(1);
}

CImg* CDimageView::Gamma(double gamma, double comp, CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	COLORREF ref;
	BYTE r, g, b;
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
    double r0,g0,b0;

	for(i=0;i<width;i++)
	{
		
		for(j=0;j<height;j++)
		{
			ref = pImg->GetPixel(i , j);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);
			r0=pow(double(r)+comp,gamma);
			g0=pow(double(g)+comp,gamma);
			b0=pow(double(b)+comp,gamma);

			if (r0<0)
				r=0;
			else if (r0>255)
				r=255;
			else
				r=r0;

			if (g0<0)
				g=0;
			else if (g0>255)
				g=255;
			else
				g=g0;

			if (b0<0)
				b=0;
			else if (b0>255)
				b=255;
			else
				b=b0;

			mImg->SetPixel(i,j,RGB(r,g,b));

		}//for j
	}//for i
	return mImg;
}

void CDimageView::OnHisteq()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();

	if(!pDoc->m_Img->IsValidate()) //ImgΪNULL���޷�����
		return;

	if(pDoc->m_OImg->IsValidate()) //ÿ���ȸ���OImg���ݣ��иı伴ȫɾ���·����ַ���Ƚϱ���
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img = Histeq(pDoc->m_OImg);
	Invalidate(1);
}


//CImg* CDimageView::Histeq(CImg* pImg)
//{
//	CImg* mImg= new CImg(*pImg);
//	int width,height,i,j;
//	double r0,g0,b0,r,g,b;
//	double pdHistr[256],pdHistg[256],pdHistb[256];
//	height=mImg->GetHeight();
//	width=mImg->GetWidthPixel();
//	mImg->GenHistrgb(pdHistr,pdHistg,pdHistb);
//	COLORREF ref;
//
//	for(i=0;i<width;i++)
//	{
//		for(j=0;j<height;j++)
//		{
//			ref = pImg->GetPixel(i , j);
//			r = GetRValue(ref);
//			g = GetGValue(ref);
//			b = GetBValue(ref);
//			
//			r0=0;
//			for (BYTE k=0; k<r; k++) r0+=*(pdHistr + k);
//			r0 = 255 * r0;
//			if (r0 < 0) r0 = 0;
//			if (r0 > 255) r0 = 255;
//
//			g0=0;
//			for (BYTE k=0; k<r; k++) g0+=*(pdHistg + k);
//			g0 = 255 * g0;
//			if (g0 < 0) g0 = 0;
//			if (g0 > 255) g0 = 255;
//
//			b0=0;
//			for (BYTE k=0; k<r; k++) b0+=*(pdHistb + k);
//			b0 = 255 * b0;
//			if (b0 < 0) b0 = 0;
//			if (b0 > 255) b0 = 255;
//
//			mImg->SetPixel(i,j,RGB(r0,g0,b0));
//		}
//	}
//	return mImg;
//}

CImg* CDimageView::Histeq(CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j;
	double r0,g0,b0,r,g,b;
	double pdHist[256];
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
	mImg->GenHist(pdHist);
	COLORREF ref;

	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			ref = pImg->GetPixel(i , j);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);
			
			r0=0;
			for (BYTE k=0; k<r; k++) r0+=*(pdHist + k);
			r0 = 255 * r0;
			if (r0 < 0) r0 = 0;
			if (r0 > 255) r0 = 255;

			g0=0;
			for (BYTE k=0; k<r; k++) g0+=*(pdHist + k);
			g0 = 255 * g0;
			if (g0 < 0) g0 = 0;
			if (g0 > 255) g0 = 255;

			b0=0;
			for (BYTE k=0; k<r; k++) b0+=*(pdHist + k);
			b0 = 255 * b0;
			if (b0 < 0) b0 = 0;
			if (b0 > 255) b0 = 255;

			mImg->SetPixel(i,j,RGB(r0,g0,b0));
		}
	}
	return mImg;
}

void CDimageView::OnSaturation()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);
	SaturationDlg* dlg= new SaturationDlg;   //ָ��Ի����ָ��
	dlg->pView=this;  //ԭ��������ȡ��ǰView��
	dlg->Create(IDD_SATURATION,NULL); //�����Ի���
	dlg->ShowWindow(SW_SHOW);//������ʾ״̬��SW_SHOW�ǰ�ԭ���ڴ�С
	dlg->MoveWindow(500,300,475,127,1);
//	//����ԭ��:BOOL MoveWindow( HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint );
////������
////��hWndָ���˴��ڵľ��
////��xָ����CWnd����ߵ���λ�á�
//��//��yָ����CWnd�Ķ�������λ�á�
//��//��nWidthָ����CWnd���¿�ȡ�
//��//��nHeightָ����CWnd���¸߶ȡ�
//��//��bRepaintָ�����Ƿ�Ҫ�ػ�CWnd��
//	//���ΪTRUE����CWnd��ͨ��������OnPaint��Ϣ�������н��յ�һ��WM_PAINT��Ϣ��
//	//����������ΪFALSE���򲻻ᷢ���κ����͵��ػ���������Ӧ���ڿͻ������ǿͻ����������������͹�������
//	//������CWnd�ƶ���¶���ĸ����ڵ��κβ��֡����������ΪFALSE��ʱ��Ӧ�ó��������ȷ��ʹCWnd�͸������б����ػ��Ĳ�����Ч���ػ���
}

void CDimageView::Saturation(int satu)
{
	CDimageDoc* pDoc= GetDocument();
	int width=pDoc->m_Img->GetWidthPixel();
	int height=pDoc->m_Img->GetHeight();
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
		{
			int color= pDoc->m_OImg->GetPixel(i,j);
			int red= color%256;
			int green= ((color-red)%(256*256))/256;
			int blue= (color-red-green)/(256*256);
			int grey=(red+blue+green)/3;  //ƽ��ô��

			red+=(red-grey)*(satu-100)/100;   //����Ϊ���ڻҶȵĲ���SAT����
			green+=(green-grey)*(satu-100)/100;
			blue+=(blue-grey)*(satu-100)/100;

			red=(red>255)?255:red;
			green=(green>255)?255:green;
			blue=(blue>255)?255:blue;

			red=(red<0)?0:red;
			green=(green<0)?0:green;
			blue=(blue<0)?0:blue;  //���д�ķ�����

			pDoc->m_Img->SetPixel(i,j,RGB(red,green,blue));    //ֱ�Ӹı�DOC��ͼ��Ȼ����£������仯Ч��
		}
	Invalidate(0);
}

void CDimageView::OnInverse()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);
	delete []pDoc->m_Img;
	pDoc->m_Img=Inverse(pDoc->m_OImg);

	Invalidate(0);
}

CImg* CDimageView::Inverse(CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width=mImg->GetWidthPixel();
	int height=mImg->GetHeight();
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			mImg->SetPixel(i,j,RGB(255,255,255)-mImg->GetPixel(i,j));
	return mImg;
}

void CDimageView::OnenSmoothf()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);
	
	
	int	nTempH;	// ģ��߶�
	int	nTempW; // ģ����
	double fTempC;// ģ��ϵ��
	int	nTempMX;// ģ������Ԫ��X����
	int	nTempMY;// ģ������Ԫ��Y����
	
	// ģ��Ԫ�����鸳��ֵ��Ĭ��Ϊƽ��ģ�壩
	double aValue[9] = {1.0, 1.0, 1.0, 
						1.0, 1.0, 1.0, 
						1.0, 1.0, 1.0,};
	
	// �����Ի���
	DlgSmooth dlg;
	
	// ��ʼ���Ի������ֵ
	dlg.m_intType = 0;
	dlg.m_iTempH  = 3;
	dlg.m_iTempW  = 3;
	dlg.m_iTempMX = 1;
	dlg.m_iTempMY = 1;
	dlg.m_fTempC  = (double) (1.0 / 9.0);
	dlg.m_fpArray = aValue;
	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlg.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	nTempH  = dlg.m_iTempH;
	nTempW  = dlg.m_iTempW;
	nTempMX = dlg.m_iTempMX;
	nTempMY = dlg.m_iTempMY;
	fTempC  = dlg.m_fTempC;

		
	delete []pDoc->m_Img;
	pDoc->m_Img = Template(nTempH, nTempW, nTempMY, nTempMX, aValue, fTempC, pDoc->m_OImg);
	Invalidate(1);
}
//	mImg1 = Template(3, 3, 1, 1, Template_HSobel, 1, pImg);
CImg* CDimageView::Template(int nTempH, int nTempW, int nTempMY, int nTempMX, 
							double *pfArray, double fCoef, CImg *pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width=mImg->GetWidthPixel();
	int height=mImg->GetHeight();
	mImg->InitPixels(0); //Ŀ��ͼ���ʼ��
	
	int i, j; //ѭ������

	//ɨ��ͼ�����ģ�����
	for(i = nTempMX; i<width- (nTempW - nTempMX) + 1; i++)
	{
		for(j = nTempMY; j<height- (nTempH - nTempMY) + 1; j++)
		{
			// (i , j)Ϊ���ĵ�
			double fResult = 0;
			for(int k=0; k<nTempW; k++)
			{
				for(int l=0; l<nTempH; l++)
				{
					//�����Ȩ��
					fResult += pImg->GetGray(i + k - nTempMX , j + l - nTempMY) * pfArray[l * nTempW + k];
				}
			}
			
			// ����ϵ��
			fResult *= fCoef;
			
			// ȡ��
			fResult = (double)fabs(fResult); //��ʱ�п��ܳ��ָ�ֵ
			
			BYTE byte;
			if(fResult!=0)
			{
			int a=1;
			}
			if(fResult > 255)
				byte = 255;
			else
				byte = fResult;
			
			mImg->SetPixel(i, j, RGB(byte, byte, byte));
		}//for j
	}//for i
	return mImg;
}




CImg* CDimageView::Midf(CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width=mImg->GetWidthPixel();
	int height=mImg->GetHeight();
	mImg->InitPixels(0); //Ŀ��ͼ���ʼ��
	int i, j, k, l;

	int nGray;
	int* pAryGray; //������������
	pAryGray = new int[9];

	// ����ɨ��ͼ�񣬽�����ֵ�˲�	
	for(i = 1; i < width - 3 + 1 + 1; i++)// ��(��ȥ��Ե����)
	{
		for(j = 1; j < height - 3 + 1 + 1; j++)// ��(��ȥ��Ե����)
		{
			// ��ȡ�˲�������
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					//ԭͼ���i + k - nFilterMY�У���j + l - nFilterMX�е�����ֵ
					nGray = pImg->GetGray(i + k - 1, j + l -1);					
					
					// ��������ֵ
					pAryGray[k*3 + l] = nGray;
				}//l
			}//k
			if(i==100)
			{
				i=100;
			}
			nGray = GetMedianValue(pAryGray, 9); //ͨ�������ȡ��ֵ
	
			mImg->SetPixel(i, j, RGB(nGray, nGray, nGray)); //����ֵ��Ϊ��Ӧ			
		}//j
	}//i

	delete [] pAryGray;
	return mImg;
}
CImg* CDimageView::adpMidf(CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width=mImg->GetWidthPixel();
	int height=mImg->GetHeight();
	mImg->InitPixels(0); //Ŀ��ͼ���ʼ��
	int i, j, k, l;

	int nGray;
	int* pAryGray; //������������
	pAryGray = new int[9];

	// ����ɨ��ͼ�񣬽�����ֵ�˲�	
	for(i = 1; i < width - 3 + 1 + 1; i++)// ��(��ȥ��Ե����)
	{
		for(j = 1; j < height - 3 + 1 + 1; j++)// ��(��ȥ��Ե����)
		{
			// ��ȡ�˲�������
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					//ԭͼ���i + k - nFilterMY�У���j + l - nFilterMX�е�����ֵ
					nGray = pImg->GetGray(i + k - 1, j + l -1);					
					
					// ��������ֵ
					pAryGray[k*3 + l] = nGray;
				}//l
			}//k
			nGray = GetMedianValue(pAryGray, 9); //ͨ�������ȡ��ֵ
			if((pImg->GetGray(i,j) == pAryGray[0]) || (pImg->GetGray(i,j) == pAryGray[8]))
				mImg->SetPixel(i, j, RGB(nGray, nGray, nGray)); //����ֵ��Ϊ��Ӧ		
			else
				nGray =pImg->GetGray(i,j);
				mImg->SetPixel(i, j, RGB(nGray,nGray,nGray));
		}//j
	}//i

	delete [] pAryGray;
	return mImg;
}

int CDimageView::GetMedianValue(int * pAryGray, int nFilterLen)
{
	int i, j;	
	int nMedianValue;
	int nTemp; //�м����
	
	//����
	for (j=0; j < nFilterLen - 1; j++)
	{
		for (i=0; i < nFilterLen - j - 1; i++)
		{
			if (pAryGray[i] > pAryGray[i + 1])
			{
				// ����λ��
				nTemp = pAryGray[i];
				pAryGray[i] = pAryGray[i + 1];
				pAryGray[i + 1] = nTemp;
			}//if
		}//for i
	}//for j
	
	// ������ֵ
	if ((nFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		nMedianValue = pAryGray[(nFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		nMedianValue = (pAryGray[nFilterLen / 2] + pAryGray[nFilterLen / 2 + 1]) / 2;
	}
	
	// ������ֵ
	return nMedianValue;
}

void CDimageView::OnenMidf()
{
	// TODO: �ڴ���������������

	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);
	CImg* m_adp = NULL;

	delete []pDoc->m_Img;
	delete []m_adp;
	pDoc->m_Img=Midf(pDoc->m_OImg);
	m_adp=adpMidf(pDoc->m_OImg);
	//pDoc->m_Img=adpMidf(pDoc->m_OImg);
	Invalidate(0);
	DlgMidf dlg;

	dlg.m_adp = m_adp;
	dlg.DoModal();
	


}

void CDimageView::OnenSobels()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=Sobel(pDoc->m_OImg);
	Invalidate(0);
}


void CDimageView::OnenLapaces()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	int nTempW = 3;
	int nTempH = 3;
	double fCoef = 1.0;
	int nTempMX = 1;
	int nTempMY = 1;
	delete []pDoc->m_Img;
	pDoc->m_Img=Template(nTempH, nTempW, nTempMY, nTempMX, Template_Laplace, fCoef, pDoc->m_OImg);
	Invalidate(0);
}

//CImg* Template(int nTempH, int nTempW, int nTempMY, int nTempMX,double *pfArray, double fCoef,CImg *pImg);
CImg* CDimageView::Sobel(CImg *pImg)
{
	CImg* mImg1= new CImg(*pImg);
	CImg* mImg2= new CImg(*pImg);
	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();

	mImg1 = Template(3, 3, 1, 1, Template_HSobel, 1, pImg);
	mImg2 = Template(3, 3, 1, 1, Template_VSobel, 1, pImg);

	//CImg* mImg = new CImg((*mImg1)+(*mImg2));
	CImg* mImg = new CImg((*mImg1)+(*mImg2));
	return mImg;
}


void CDimageView::Onenhigh()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=adpthreshold(pDoc->m_OImg);
	Invalidate(0);
}


CImg* CDimageView::adpthreshold(CImg* pImg)
{
	CImg* mImg= new CImg(*pImg);
	int width,height,i,j,k,l;
	COLORREF ref;
	BYTE gray,nGray;
	height=mImg->GetHeight();
	width=mImg->GetWidthPixel();
	//int pAryGray[9]; //������������
	int avergray =0;
	for(i=1;i<width-1;i++)
	{		
		for(j=1;j<height-1;j++)
		{
			gray= pImg->GetGray(i , j);
			avergray=0;
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					nGray = pImg->GetGray(i + k - 1, j + l -1);					
				// ��������ֵ
					//pAryGray[k*3 + l] = nGray;
					avergray+=nGray;
				}//l
			}//k
			//nGray = GetMedianValue(pAryGray, 9); //ͨ�������ȡ��ֵ
			nGray = avergray/9;
			gray = gray>=nGray?255:0;
			
			mImg->SetPixel(i,j,RGB(gray,gray,gray));
		}//for j
	}//for i
	return mImg;
}


void CDimageView::OnRobertEdge()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=RobertEdge(pDoc->m_OImg);
	Invalidate(0);
}


CImg* CDimageView::RobertEdge(CImg* pImg)
{
	double cfRobertCW[4]={-1 , 0, 0 , 1};
	double cfRobertCCW[4]={0 , -1, 1 , 0};

	CImg* mImg1= new CImg(*pImg);
	CImg* mImg2= new CImg(*pImg);

	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();

	mImg1 = Template(2, 2, 0, 0, cfRobertCW, 1, pImg);
	mImg2 = Template(2, 2, 0, 0, cfRobertCCW, 1, pImg);

	CImg* nImg = new CImg((*mImg1)+(*mImg2));
//	CImg* mImg = new CImg((*mImg2));
	
    CImg* mImg= Autothres (nImg);
	return mImg;
}


void CDimageView::OnAutothre()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=Autothres(pDoc->m_OImg);
	Invalidate(0);
}


CImg* CDimageView::Autothres(CImg* pImg)
{	
	CImg* mImg= new CImg(*pImg);
	int nThreshold;
	
	// ֱ��ͼ����
	int nHistogram[256] = { 0 };
	int i, j;
	
	BYTE bt;
	int nDiffRet;
	int nMin = 255;
	int nMax = 0;
	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();
	int nMaxIter=100;
//	BYTE gray;

	// ɨ��ͼ��,����������С�ҶȺ�ֱ��ͼ
	for(j = 0; j < height; j ++)
	{
		for(i=0; i<width; i++)
		{
//			gray = pImg->GetGray(i,j);
			bt = pImg->GetGray(i,j);
			
			if(bt < nMin)
				nMin = bt;
			if(bt > nMax)
				nMax = bt;
			
			nHistogram[bt] ++;
			
		}
	}
	
	int nTotalGray = 0; //�Ҷ�ֵ�ĺ�
	int nTotalPixel = 0; //�������ĺ�
	int nNewThreshold = (nMax + nMin)/2; //��ʼ��ֵ
	
	nDiffRet = nMax - nMin;

	if (nMax == nMin)
		nThreshold = nNewThreshold;

	else
	{
		nThreshold = 0;
		
		// ������ʼ,ֱ�����������ﵽ100������ֵ����һ�ֵõ�����ֵ��ȣ���������
		for(int nIterationTimes = 0; nThreshold != nNewThreshold && nIterationTimes < nMaxIter; nIterationTimes ++)
		{
			nThreshold = nNewThreshold;
			nTotalGray = 0;
			nTotalPixel = 0;

			//����ͼ����С�ڵ�ǰ��ֵ���ֵ�ƽ���Ҷ�
			for(i=nMin; i<nThreshold; i++)
			{
				nTotalGray += nHistogram[i]*i;
				nTotalPixel += nHistogram[i];
			}
			int nMean1GrayValue = nTotalGray/nTotalPixel;


			nTotalGray = 0;
			nTotalPixel = 0;
			
			//����ͼ���д��ڵ�ǰ��ֵ���ֵ�ƽ���Ҷ�
			for(i=nThreshold + 1; i<=nMax; i++)
			{
				nTotalGray += nHistogram[i]*i;
				nTotalPixel += nHistogram[i];
			}
			int nMean2GrayValue = nTotalGray/nTotalPixel;
	
			nNewThreshold = (nMean1GrayValue + nMean2GrayValue)/2; //������µ���ֵ
			nDiffRet = abs(nMean1GrayValue - nMean2GrayValue);
		}



	}
//---------------------------Set threshold----------------------//
	for(j = 0; j < height; j ++)
	{
		for(i=0; i<width; i++)
		{
//			gray = pImg->GetGray(i,j);
			bt = pImg->GetGray(i,j);
			
			if(bt < nThreshold)
				bt=0;
			else
				bt = 255;
			
			mImg->SetPixel(i,j,RGB(bt,bt,bt));
		}
	}
	return mImg;
}


void CDimageView::OnSobelEdge()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=SobelEdge(pDoc->m_OImg);
	Invalidate(0);
}


CImg* CDimageView::SobelEdge(CImg* pImg)
{
	double cfSobelH[9] = {
		-1, -1, -1,
		 0,  0,  0,
		 1,  1,  1 };
	// ��ֱ��Ե
	double cfSobelV[9] = {
		-1,  0,  1,
		-1,  0,  1,
		-1,  0,  1 };
	// 45�ȱ�Ե
	double cfSobelCW[9] = {
		-1, -1,  0,
		-1,  0,  1,
		 0,  1,  1 };
	// 135�ȱ�Ե
	double cfSobelCCW[9] = {
		 0,  1,  1,
		-1,  0,  1,
		-1, -1,  0 };

	CImg* mImg1= new CImg(*pImg);
	CImg* mImg2= new CImg(*pImg);
	CImg* mImg3= new CImg(*pImg);
	CImg* mImg4= new CImg(*pImg);

	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();

	mImg1 = Template(3, 3, 1, 1, cfSobelH, 1, pImg);
	mImg2 = Template(3, 3, 1, 1, cfSobelV, 1, pImg);
	mImg3 = Template(3, 3, 1, 1, cfSobelCW, 1, pImg);
	mImg4 = Template(3, 3, 1, 1, cfSobelCCW, 1, pImg);

	CImg* nImg = new CImg((*mImg1)+(*mImg2)+(*mImg3)+(*mImg4));
//	CImg* nImg = new CImg((*mImg1)+(*mImg2));
//	CImg* mImg = new CImg((*mImg2));
	
    CImg* mImg= Autothres (nImg);
	return mImg;
}


void CDimageView::OnPrewittEdge()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=PrewittEdge(pDoc->m_OImg, 0);
	Invalidate(0);
}


CImg* CDimageView::PrewittEdge(CImg* pImg,int index)
{
	double cfPrewittH[9] = {
		-1, -2, -1,
		 0,  0,  0,
		 1,  2,  1 };
	// ��ֱ��Ե
	double cfPrewittV[9] = {
		-1,  0,  1,
		-2,  0,  2,
		-1,  0,  1 };
	// 45�ȱ�Ե
	double cfPrewittCW[9] = {
		-2, -1,  0,
		-1,  0,  1,
		 0,  1,  2 };
	// 135�ȱ�Ե
	double cfPrewittCCW[9] = {
		 0,  1,  2,
		-1,  0,  1,
		-2, -1,  0 };

	CImg* mImg1= new CImg(*pImg);
	CImg* mImg2= new CImg(*pImg);
	CImg* mImg3= new CImg(*pImg);	
	CImg* mImg4= new CImg(*pImg);

	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();

	mImg1 = Template(3, 3, 1, 1, cfPrewittH, 1, pImg);
	mImg2 = Template(3, 3, 1, 1, cfPrewittV, 1, pImg);
	mImg3 = Template(3, 3, 1, 1, cfPrewittCW, 1, pImg);
	mImg4 = Template(3, 3, 1, 1, cfPrewittCCW, 1, pImg);

	CImg* nImg = new CImg((*mImg1)+(*mImg2)+(*mImg3)+(*mImg4));
//	CImg* mImg = new CImg((*mImg2));
	
    CImg* mImg= Autothres (nImg);
	switch(index)
	{
		case 0:return mImg;
		case 1:return Autothres (mImg1);
		case 2:return Autothres (mImg2);
		case 3:return Autothres (mImg3);
		case 4:return Autothres (mImg4);
	}
}


void CDimageView::OnLogEdge()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=LogEdge(pDoc->m_OImg);
	Invalidate(0);
}


CImg* CDimageView::LogEdge(CImg* pImg)
{
	double cfLog[25]={0, 0, -1, 0, 0,
						0, -1, -2, -1, 0,
						-1, -2, 16, -2, -1,
						0, -1, -2, -1, 0,
						0, 0, -1, 0, 0};

	CImg* mImg1= new CImg(*pImg);

	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();

	mImg1 = Template(5, 5, 2, 2, cfLog, 1, pImg);

//	CImg* mImg = new CImg((*mImg2));
	
    CImg* mImg= Autothres (mImg1);
	return mImg;
}


void CDimageView::OnCanny()
{
	// TODO: �ڴ���������������
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);

	delete []pDoc->m_Img;
	pDoc->m_Img=Canny(pDoc->m_OImg);
	Invalidate(0);
}


CImg* CDimageView::Canny(CImg* pImg)
{
	// �������ݶ�ֵ

	// ʹ��Prewittģ�������������ϵ��ݶ�ֵ
	CImg* imgGH=PrewittEdge(pImg,1);
	CImg* imgGV=PrewittEdge(pImg,2);
	CImg* imgGCW=PrewittEdge(pImg,3);
	CImg* imgGCCW=PrewittEdge(pImg,4);
	CImg* imgGratitude = new CImg(*pImg);
	imgGratitude->InitPixels(0);

	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();
	// ����ݶȷ���
	BYTE * pbDirection = new BYTE [height * width];

	memset(pbDirection, 0,height * width * sizeof(BYTE));

	// Ѱ��ÿ�������ݶȷ���д���Ӧ������ݶ�ֵ

	for (int i=0; i<height; i++)
	{
		for (int j=0; j<width; j++)
		{
			BYTE gray = 0;

			if (imgGH->GetGray(j, i) > gray)
			{
				gray = imgGH->GetGray(j, i);
				pbDirection[i * width + j] = 1;
				imgGratitude->SetPixel(j, i, RGB(gray, gray, gray));
			}

			if (imgGV->GetGray(j, i) > gray)
			{
				gray = imgGV->GetGray(j, i);
				pbDirection[i * width + j] = 2;
				imgGratitude->SetPixel(j, i, RGB(gray, gray, gray));
			}

			if (imgGCW->GetGray(j, i) > gray)
			{
				gray = imgGCW->GetGray(j, i);
				pbDirection[i * width + j] = 3;
				imgGratitude->SetPixel(j, i, RGB(gray, gray, gray));
			}

			if (imgGCCW->GetGray(j, i) > gray)
			{
				gray = imgGCCW->GetGray(j, i);
				pbDirection[i * width + j] = 4;
				imgGratitude->SetPixel(j, i, RGB(gray, gray, gray));
			}
		}
	}

	// ��ֵ��ʱ����ǰ��Ķ���
	CImg *pImgThreL = imgGH, *pImgThreH = imgGV;

	// �����ֵ��������δ������ֵ�������ȡ�������ֵ
	int bThreH;
	int bThreL;

	bThreH = 1.2 * imgGratitude->DetectThreshold(100);
	bThreL = 0.4 * bThreH;
		

	// ������ݶ�ͼ�񰴸ߵ�ֵ�ֱ������ֵ��
	imgGratitude->Threshold(pImgThreL, bThreL);
	imgGratitude->Threshold(pImgThreH, bThreH);

	// ��ʼ��Ŀ��ͼ��
	CImg* mImg = new CImg(*pImg);
	mImg->InitPixels(0);
	
	// ���ݵ���ֵͼ���ڸ���ֵͼ���Ͻ��б߽��޲�
	for (int i=1; i<height-1; i++)
	{
		for (int j=1; j<width-1; j++)
		{
			if (pImgThreH->GetGray(j, i))
			{
				// ����ֵͼ���Ϸ��ֵ�ֱ��ȷ��
				mImg->SetPixel(j, i, RGB(255, 255, 255));
				
				// �����ݶ�������ϵ�����
				switch ( pbDirection[i * width + j] ) {
					case 1:	// ˮƽ����
						if (pImgThreL->GetGray(j+1, i))
						{
							pImgThreH->SetPixel(j+1, i, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j-1, i))
						{
							pImgThreH->SetPixel(j-1, i, RGB(255, 255, 255));
						}
						break;

					case 2:	// ��ֱ����
						if (pImgThreL->GetGray(j, i+1))
						{
							pImgThreH->SetPixel(j, i+1, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j, i-1))
						{
							pImgThreH->SetPixel(j, i-1, RGB(255, 255, 255));
						}
						break;

					case 3:	// 45�ȷ���
						if (pImgThreL->GetGray(j+1, i-1))
						{
							pImgThreH->SetPixel(j+1, i-1, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j-1, i+1))
						{
							pImgThreH->SetPixel(j-1, i+1, RGB(255, 255, 255));
						}
						break;

					case 4:	// 135�ȷ���
						if (pImgThreL->GetGray(j+1, i+1))
						{
							pImgThreH->SetPixel(j+1, i+1, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j-1, i-1))
						{
							pImgThreH->SetPixel(j-1, i-1, RGB(255, 255, 255));
						}
						break;
				}
			}//if
		}//for j
	}//for i



	delete pbDirection;

	return mImg;
}


void CDimageView::OnFwdDwt()
{
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);//���浱ǰ�ĵ���ת�÷��ؿռ�
	delete []pDoc->m_Img;

	pDoc->m_Img=Dwtstep(pDoc->m_OImg,0);

	Invalidate(0);
}


void CDimageView::OnBwdDwt()
{
	CDimageDoc* pDoc= GetDocument();
	if(!pDoc->m_Img->IsValidate())
		return;
	if(pDoc->m_OImg->IsValidate())
	{
		delete []pDoc->m_OImg;
		pDoc->m_OImg=NULL;
	}
	pDoc->m_OImg= new CImg(*pDoc->m_Img);//���浱ǰ�ĵ���ת�÷��ؿռ�
	delete []pDoc->m_Img;

	pDoc->m_Img=Dwtstep(pDoc->m_OImg,1);

	Invalidate(0);
}



CImg* CDimageView::Dwtstep(CImg* pImg, int Inv)
{
	// ��ʼ��Ŀ��ͼ��
	CImg* mImg = new CImg(*pImg);
	mImg->InitPixels(0);
	int width=pImg->GetWidthPixel();
	int height=pImg->GetHeight();
	int nMaxWLevel = Log2(width);
	int nMaxHLevel = Log2(height);
	int nMaxLevel;

	if (width == 1<<nMaxWLevel && height == 1<<nMaxHLevel)
		nMaxLevel = min(nMaxWLevel, nMaxHLevel);

	// ���С���任�Ĵ洢�ڴ滹û�з��䣬�������ڴ�,��double����graylevel
	if(!Dwttemp){			
		Dwttemp = new double[width*height];

		// ��ͼ�����ݷ���m_pDbImage�� 
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
				Dwttemp[i * width + j] = double(pImg->GetGray(j,i));
		}
	}

	if (!DWTStep_2D(Dwttemp, nMaxWLevel-Curdpth, nMaxHLevel-Curdpth,nMaxWLevel, nMaxHLevel, Inv))
		return FALSE;
	
	if (Inv)
		Curdpth --;
	// �����1
	else
		Curdpth ++;

	BYTE gray;
	//��double��gray����ת��24λbmp(gray,gray,gray)
	int lfw = width>>Curdpth, lfh = height>>Curdpth; //�߶�����
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++){
				if(i<lfh && j<lfw){ //��ƵX��Ƶ��>0ֱ��ת
					gray = BYTE(Dwttemp[i * width + j]);
					mImg->SetPixel(j,i,RGB(gray,gray,gray));
				}
				else{  //С���㲿�� +128��ʾ
					gray = BYTE(Dwttemp[i * width + j]+128);
					mImg->SetPixel(j,i,RGB(gray,gray,gray));
				}
			}

		}
	return mImg;
}



