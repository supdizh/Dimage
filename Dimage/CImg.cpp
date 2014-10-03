// Img.cpp: implementation of the CImg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CImg.h"

#include <math.h>
#include "cimg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CImg::CImg()
{
	m_pBMIH = NULL;
	m_lpvColorTable = NULL;

	m_lpData = NULL;
}

BOOL CImg::operator == (CImg& gray)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	if(nHeight != gray.GetHeight())
		return false;

	if(nWidth != gray.GetWidthPixel())
		return false;


	for(int i=0; i<nHeight; i++)
	{
		for(int j=0; j<nWidth; j++)
		{
			if( GetGray(j, i) != gray.GetGray(j, i) )
				return false;
		}
	}

	return true;
}

CImg CImg::operator & (CImg& gray)
{
	CImg grayRet = *this;
	
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	
	if((nHeight != gray.GetHeight())||(nWidth != gray.GetWidthPixel()))
	{
		CString t("图像大小不同，无法比较！");
		MessageBox(0,t,0,MB_OK);
		return grayRet;
	}

	// 两幅图像的与
	for(int i=0; i<nHeight; i++)
	{
		for(int j=0; j<nWidth; j++)
		{
			if(gray.GetGray(j, i) == 255)
				grayRet.SetPixel(j, i, RGB(255, 255, 255));
		}
	}

	return grayRet;	
}

CImg CImg::operator | (CImg& gray)
{
	CImg grayRet = *this;
	
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	
	if((nHeight != gray.GetHeight())||(nWidth != gray.GetWidthPixel()))
	{
		CString t("图像大小不同，无法比较！");
		MessageBox(0,t,0,MB_OK);
		return grayRet;
	}


	// 两幅图像的或
	for(int i=0; i<nHeight; i++)
	{
		for(int j=0; j<nWidth; j++)
		{
			if(gray.GetGray(j, i) == 0)
				grayRet.SetPixel(j, i, RGB(0, 0, 0));
		}
	}

	return grayRet;
}

/*******************
CImg CImg::operator + (CImg gray)

功能：图像按位加

参数：
	CImg 对象

返回值:
	CImg 相加后的 CImg 类图像对象
*******************/

CImg CImg::operator + (CImg gray)
{
	CImg grayRet; //返回图像
	grayRet = *this;
	BYTE dgray;
	//取得图像的高和宽
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;//循环变量
	
	//逐行扫描图像
	for(j=0; j<nHeight; j++)
	{
		for(i=0; i<nWidth; i++)
		{
			//按位相加
			dgray = (grayRet.GetGray(i,j)+gray.GetGray(i,j))/2;
			grayRet.SetPixel(i,j,RGB(dgray,dgray,dgray));

		}// j
	}// i
		
	
	
	return grayRet;
}


void CImg::operator = (CImg& gray)
{
	CleanUp();

	m_nColorTableEntries = gray.m_nColorTableEntries;


	if(gray.m_pBMIH != NULL)
	{
		m_pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4];
		memcpy(m_pBMIH, gray.m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

		if(m_nColorTableEntries != 0)
		{
			m_lpvColorTable = m_pBMIH + 1;
		}
	}

	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	m_lpData = new LPBYTE[m_pBMIH->biHeight];
	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		m_lpData[i] = new BYTE[nWidthBytes];
		memcpy(m_lpData[i], gray.m_lpData[i], nWidthBytes);
	}
}

CImg::CImg(CImg& gray)
{	
	m_pBMIH = NULL;
	m_lpvColorTable = NULL;

	m_nColorTableEntries = gray.m_nColorTableEntries;
	
	if(gray.m_pBMIH != NULL)
	{
		m_pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4];
		memcpy(m_pBMIH, gray.m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

		if(m_nColorTableEntries != 0)
		{
			m_lpvColorTable = m_pBMIH + 1;
		}
	}

	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	m_lpData = new LPBYTE[m_pBMIH->biHeight];
	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		m_lpData[i] = new BYTE[nWidthBytes];
		memcpy(m_lpData[i], gray.m_lpData[i], nWidthBytes);
	}
}

CImg::~CImg()
{
	CleanUp();
}

void CImg::CleanUp()
{	
	if(m_lpData != NULL)
	{
		int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
		for(int i=0; i<m_pBMIH->biHeight; i++)
		{
			delete[] m_lpData[i];
		}
		delete[] m_lpData;
	}

	if(m_pBMIH != NULL)
	{
		delete[] m_pBMIH;
		m_pBMIH = NULL;
	}
}

/**************************************************
void CImg::ImResize(int nHeight, int nWidth)

功能：
	用给定的大小重新初始化CImg对象

限制：
	CImg对象必须已经包含有效的图像数据，否则将出错

参数：
	int nHeight
		重新初始化成的宽度
	int nWidth
		重新初始化成的高度
返回值：
	无
***************************************************/
void CImg::ImResize(int nHeight, int nWidth)
{
	int i; //循环变量
	//释放图像数据空间
	for(i=0; i<m_pBMIH->biHeight; i++)
	{
		delete[] m_lpData[i];
	}
	delete[] m_lpData;

	//更新信息头中的相应内容
	m_pBMIH->biHeight = nHeight; //更新高度
	m_pBMIH->biWidth = nWidth; //更新宽度

	//重新分配数据空间
	m_lpData = new LPBYTE [nHeight];
	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	for(i=0; i<nHeight; i++)
	{
		m_lpData[i] = new BYTE [nWidthBytes];
	}
}

/**************************************************
void CImg::InitPixels(BYTE color)
功能：
	用给定的颜色值初始化图像的所有像素

限制：
	只能使用灰度值提供颜色值，即只能初始化为某种灰色
参数：
	BYTE color：指定的用来初始化图像的灰度值
返回值：
	无
***************************************************/
void CImg::InitPixels(BYTE color)
{
	//获得图像高、宽
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;//行、列循环变量

	//逐行扫描图像，依次对每个像素设置color灰度
	if(m_lpData != NULL)
	{
		for(i=0; i<GetHeight(); i++)
		{
			for(j=0; j<GetWidthPixel(); j++)	
			{
				SetPixel(j, i, RGB(color, color, color));
			}//for j
		}//for i
	}
}


/**************************************************
BOOL CImg::AttachFromFile(LPCTSTR lpcPathName)

功能：
	打开指定的图像文件并附加到CImg对象上
限制：
	只能处理位图图像

参数：LPCTSTR lpcPathName
		欲打开文件的完整路径
返回值：
	BYTE类型，TRUE为成功，FALSE为失败
***************************************************/
BOOL CImg::AttachFromFile(LPCTSTR lpcPathName)
{
	// 使用CFile对象简化操作
	CFile file;
	if(!file.Open(lpcPathName, CFile::modeRead|CFile::shareDenyWrite))
		return FALSE;

	BOOL bSuc = AttachFromFile(file);

	file.Close();
	return bSuc;
}

/**************************************************
BOOL CImg::AttachFromFile(CFile &file)

功能：
	打开指定的图像文件并附加到CImg对象上
限制：
	只能处理位图图像

参数：
	CFile &file
		欲打开的CFile对象
返回值：
	BYTE类型，TRUE为成功，FALSE为失败
***************************************************/

BOOL CImg::AttachFromFile(CFile &file)
{
	// 文件数据
	LPBYTE  *lpData;
	// 位图信息头
	BITMAPINFOHEADER *pBMIH;
	// 颜色表指针
	LPVOID lpvColorTable = NULL;
	// 颜色表颜色数目
	int nColorTableEntries;

	BITMAPFILEHEADER bmfHeader;

	// 读取文件头
	if(!file.Read(&bmfHeader, sizeof(bmfHeader)))
		return FALSE;

	
	// 检查开头两字节是否为BM
	if(bmfHeader.bfType != MAKEWORD('B', 'M'))
	{
		return FALSE;
	}

	// 读取信息头
	pBMIH = (BITMAPINFOHEADER*)new BYTE[bmfHeader.bfOffBits - sizeof(bmfHeader)];
	if(!file.Read(pBMIH, bmfHeader.bfOffBits - sizeof(bmfHeader)))
	{
		delete pBMIH;
		return FALSE;
	}

	// 定位到颜色表
	nColorTableEntries = 
		(bmfHeader.bfOffBits - sizeof(bmfHeader) - sizeof(BITMAPINFOHEADER))/sizeof(RGBQUAD);
	if(nColorTableEntries > 0)
	{
		lpvColorTable = pBMIH + 1;
	}

	pBMIH->biHeight = abs(pBMIH->biHeight);

	// 读取图像数据,WIDTHBYTES宏用于生成每行字节数
	int nWidthBytes = WIDTHBYTES((pBMIH->biWidth)*pBMIH->biBitCount);

	// 申请biHeight个长度为biWidthBytes的数组,用他们来保存位图数据
	lpData = new LPBYTE[(pBMIH->biHeight)];
	for(int i=0; i<(pBMIH->biHeight); i++)
	{
		lpData[i] = new BYTE[nWidthBytes];
		file.Read(lpData[i], nWidthBytes);
		
	}

	// 更新数据
	CleanUp();

	m_lpData = lpData;
	m_pBMIH = pBMIH;
	m_lpvColorTable = lpvColorTable;
	m_nColorTableEntries = nColorTableEntries;
	

	return TRUE;
}

BOOL CImg::SaveToFile(LPCTSTR lpcPathName)
{
	if(!IsValidate())
		return FALSE;

	CFile file;
	if(!file.Open(lpcPathName, CFile::modeRead|CFile::modeWrite|CFile::modeCreate))
	{
		return FALSE;
	}

	BOOL bSuc = SaveToFile(file);
	file.Close();

	return bSuc;
}

/**************************************************
BOOL CImg::SaveToFile(CFile &file)

功能：
	把CImg实例中的图像数据保存到指定的图像文件
限制：
	只能处理位图图像

参数：
	CFile &file
		欲保存到的CFile对象
返回值：
	BYTE类型，TRUE为成功，FALSE为失败
***************************************************/
BOOL CImg::SaveToFile(CFile &file)
{	
	// 判断是否有效
	if(!IsValidate())
		return FALSE;	
	
		
	// 构建BITMAPFILEHEADER结构
	BITMAPFILEHEADER bmfHeader = { 0 };
	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);


	bmfHeader.bfType = MAKEWORD('B', 'M');
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) 
				+ sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4;

	bmfHeader.bfSize = bmfHeader.bfOffBits + m_pBMIH->biHeight * nWidthBytes;
	
	// 向文件中写入数据
	file.Write(&bmfHeader, sizeof(bmfHeader));
	file.Write(m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		file.Write(m_lpData[i], nWidthBytes);
	}
	

	return TRUE;
}


/**************************************************
BOOL CImg::Draw(CDC* pDC)

功能：
	在给定的设备上下文环境中将CImg对象中存储的图像绘制到屏幕上

限制：
	无

参数：
	CDC * pDC
		指定的设备上下文环境的指针
返回值：
	布尔类型，TRUE为成功，FALSE为失败
***************************************************/
BOOL CImg::Draw(CDC* pDC)
{
	if(m_pBMIH == NULL)
		return FALSE;

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
	
		::SetDIBitsToDevice(*pDC, 0/*0*/, 0/*0*/, m_pBMIH->biWidth, 
			m_pBMIH->biHeight, 0/*0*/, 0/*0*/, i, 1, m_lpData[i], (BITMAPINFO*)m_pBMIH, DIB_RGB_COLORS);
	}

	return TRUE;
}


BOOL CImg::Draw(CDC* pDC, CRect rt)
{
	if(m_pBMIH == NULL)
		return FALSE;

	if(rt.IsRectNull())
	{
		rt.right = m_pBMIH->biWidth;
		rt.bottom = m_pBMIH->biHeight;
	}

	if(rt.Width() > m_pBMIH->biWidth)
	{
		rt.right = rt.left + m_pBMIH->biWidth;
	}

	if(rt.Height() > m_pBMIH->biHeight)
	{
		rt.bottom = rt.top + m_pBMIH->biHeight;
	}

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
	
			::SetDIBitsToDevice(*pDC, rt.left, rt.top, rt.Width(), 
				rt.Height(), 0, 0, i, 1, m_lpData[i], (BITMAPINFO*)m_pBMIH, DIB_RGB_COLORS);
	
	}

	return TRUE;
}

/**************************************************
void CImg::SetPixel(int x, int y, COLORREF color)

功能：
	设定指定坐标位置像素的颜色值

限制：
	无

参数：
	int x, int y
		指定的像素横、纵坐标值
	COLORREF
		欲设定的指定位置的颜色值，RGB形式给出
返回值：
	无
***************************************************/
void CImg::SetPixel(int x, int y, COLORREF color)
{
	if(m_pBMIH->biBitCount == 8)			// 256色图
	{
		m_lpData[m_pBMIH->biHeight - y - 1][x] = GetRValue(color);
	}
	else if(m_pBMIH->biBitCount == 1)		// 单色图
	{
		BYTE Color = GetRValue(color);


		// 令0代表黑
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			Color = !Color;

		y = m_pBMIH->biHeight - y - 1;
		
		// 黑色点
		if (Color == 0)  // 设置对应位为0
		{
			if(x % 8==0)
			{
				m_lpData[y][x/8] &= 0x7F;  
			}
			else if(x % 8==1)
			{
				m_lpData[y][x/8] &= 0xBF;
			}
			else if(x % 8==2)
			{
				m_lpData[y][x/8] &= 0xDF;
			}
			else if(x % 8==3)
			{
				m_lpData[y][x/8] &= 0xEF;
			}
			else if(x % 8==4)
			{
				m_lpData[y][x/8] &= 0xF7;
			}
			else if(x % 8==5)
			{
				m_lpData[y][x/8] &= 0xFB;
			}
			else if(x % 8==6)
			{
				m_lpData[y][x/8] &= 0xFD;
			}
			else if(x % 8==7)
			{
				m_lpData[y][x/8] &= 0xFE;
			}
		}
		else // 白色点
		{
			if(x % 8==0)
			{
				m_lpData[y][x/8] |= 0x80;
			}
			else if(x % 8==1)
			{
				m_lpData[y][x/8] |= 0x40;
			}
			else if(x % 8==2)
			{
				m_lpData[y][x/8] |= 0x20;
			}
			else if(x % 8==3)
			{
				m_lpData[y][x/8] |= 0x10;
			}
			else if(x % 8==4)
			{
				m_lpData[y][x/8] |= 0x08;
			}
			else if(x % 8==5)
			{
				m_lpData[y][x/8] |= 0x04;
			}
			else if(x % 8==6)
			{
				m_lpData[y][x/8] |= 0x02;
			}
			else if(x % 8==7)
			{
				m_lpData[y][x/8] |= 0x01;
			}
		}
	}
	else if(m_pBMIH->biBitCount == 24)
	{
		BYTE byte1=GetBValue(color);
		BYTE byte2=GetGValue(color);
		BYTE byte3=GetRValue(color);
		m_lpData[m_pBMIH->biHeight - y - 1][x*3] = GetBValue(color);
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 1] = GetGValue(color);
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 2] = GetRValue(color);
		int a=1;
	}
}



//void CImg::Rectangle(POINT ptLT, POINT ptRB)
//{
//	if(m_pBMIH->biBitCount == 8)
//	{
//		for(int j = ptLT.y; j < ptRB.y + 1; j++)
//		{
//			for(int i = ptLT.x; i < ptRB.x + 1; i++)
//			{
//				if(j == ptLT.y || j == ptRB.y || i == ptLT.x || i ==  ptRB.x)
//				{
//					SetPixel(i, j, 1);
//				}
//			}
//		}
//	}
//	else if(m_pBMIH->biBitCount == 24)
//	{
//		for(int j = ptLT.y; j < ptRB.y + 1; j++)
//		{
//			for(int i = ptLT.x; i < ptRB.x + 1; i++)
//			{
//				if(j == ptLT.y || j == ptRB.y || i == ptLT.x || i ==  ptRB.x)
//				{
//					SetPixel(i, j, RGB(255, 0, 0));
//				}
//			}
//		}
//	}
//	else
//	{
//		for(int j = ptLT.y; j < ptRB.y + 1; j++)
//		{
//			for(int i = ptLT.x; i < ptRB.x + 1; i++)
//			{
//				if(j == ptLT.y || j == ptRB.y || i == ptLT.x || i ==  ptRB.x)
//				{
//					SetPixel(i, j, RGB(0, 0, 0));
//				}
//			}
//		}
//	}
//}
//void CImg::Rectangle(POINT ptLT, int r)
//{
//	Rectangle(ptLT.x, ptLT.y, r);
//}
//void CImg::Rectangle(int x, int y, int r)
//{
//
//	if(m_pBMIH->biBitCount == 24)
//	{
//		for(int j = y - r; j < y + r + 1; j++)
//		{
//			for(int i = x - r; i < x + r + 1; i++)
//			{
//				if(j == y - r || j == y + r || i == x - r || i == x + r)
//				{
//					SetPixel(i, j, RGB(255, 0, 0));
//				}
//			}
//		}
//	}
//	else if(m_pBMIH->biBitCount == 1)
//	{
//		for(int j = y - r; j < y + r + 1; j++)
//		{
//			for(int i = x - r; i < x + r + 1; i++)
//			{
//				if(j == y - r || j == y + r || i == x - r || i == x + r)
//				{
//					SetPixel(i, j, RGB(0, 0, 0));
//				}
//			}
//		}
//	}
//	else
//	{
//		for(int j = y - r; j < y + r + 1; j++)
//		{
//			for(int i = x - r; i < x + r + 1; i++)
//			{
//				if(j == y - r || j == y + r || i == x - r || i == x + r)
//				{
//					SetPixel(i, j, RGB(0, 0, 0));
//				}
//			}
//		}
//	}
//}

//void CImg::Line(POINT ptStart, int nLen, int nWide, BOOL bHor)
//{
//	int i, j;
//	DWORD dw = RGB(0, 0, 0);
//	if(m_pBMIH->biBitCount == 24)
//		dw = RGB(255, 0, 0);
//	else if(m_pBMIH->biBitCount == 8)
//		dw = RGB(1, 1, 1);
//
//	if(bHor)
//	{
//		for(i=ptStart.x; i<=nLen+ptStart.x; i++)
//		{
//			for(j=0; j<nWide; j++)
//			{
//				SetPixel(i, ptStart.y + j, dw);
//			}
//		}
//	}
//	else
//	{
//		for(j=ptStart.y; j<=nLen+ptStart.y; j++)
//		{
//			for(i=0; i<nWide; i++)
//			{
//				SetPixel(ptStart.x + i, j, dw);
//			}
//		}
//	}
//}


//void CImg::Line(POINT ptStart, POINT ptEnd)
//{
//	CVector2D line;
//
//	// 获取距离
//	int l = sqrt((double)(ptStart.x - ptEnd.x) * (ptStart.x - ptEnd.x)
//							+ (ptStart.y - ptEnd.y) * (ptStart.y - ptEnd.y));
//	POINT ptNext = ptStart;
//	
//	if(m_pBMIH->biBitCount == 24)
//	{
//		for(int i=0; i<=l; i++)
//		{
//			line.GetDiscretionalNextPos(ptStart, ptEnd, l, i, ptNext);
//			SetPixel(ptNext.x, ptNext.y, RGB(255, 0, 0));
//		}
//	}
//	else if(m_pBMIH->biBitCount == 1)
//	{
//		for(int i=0; i<=l; i++)
//		{
//			line.GetDiscretionalNextPos(ptStart, ptEnd, l, i, ptNext);
//			SetPixel(ptNext.x, ptNext.y, RGB(0, 0, 0));
//		}
//	}
//	else
//	{
//		for(int i=0; i<=l; i++)
//		{
//			line.GetDiscretionalNextPos(ptStart, ptEnd, l, i, ptNext);
//			SetPixel(ptNext.x, ptNext.y, RGB(1, 1, 1));
//		}
//	}
//}


// 索引图像转灰度图像
bool CImg::Index2Gray()
{
	int i;

	if (!IsIndexedImg()) return false;
	RGBQUAD *table = (RGBQUAD*)m_lpvColorTable;

	m_pBMIH->biBitCount = 8;

	// 更新颜色数据
	for (i=0; i<GetHeight(); i++)
	{
		for (int j=0; j<GetWidthPixel(); j++)
		{
			RGBQUAD rgb = *(table+GetGray(j, i));
			BYTE gray = rgb.rgbBlue * 0.114 + rgb.rgbGreen * 0.587 + rgb.rgbRed * 0.299 + 0.5;
			SetPixel(j, i, RGB(gray, gray, gray));
		}
	}
	
	// 更新颜色表
	for (i=0; i<256; i++)
	{
		(table + i)->rgbBlue = i;
		(table + i)->rgbGreen = i;
		(table + i)->rgbRed = i;
		(table + i)->rgbReserved = 0;
	}

	m_nColorTableEntries = 256;
	return true;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************
CImg CImg::operator - (CImg gray)

功能：图像按位减

参数：
	CImg 对象

返回值:
	CImg： 相减后的 CImg 类图像对象
*******************/
CImg CImg::operator - (CImg &gray)
{
	CImg grayRet; //返回图像
	grayRet = *this;

	//取得图像的高和宽
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j, color;

	//逐行扫描图像
	for(j=0; j<nHeight; j++)
	{
		for(i=0; i<nWidth; i++)
		{
			//按位相加
			color=(grayRet.GetPixel(i,j)-gray.GetPixel(i,j)+255)/2;
			grayRet.SetPixel(i,j,0);

		}// j
	}// i
			
	return grayRet;
}



BOOL CImg::DrawPart(CDC* pDC, int x_order, int y_order, int size)
{
	return 0;
}


void CImg::GenHist(double * pdHist, int n)
{
	// 计算分段因子
	double dDivider;

	memset(pdHist, 0, n * sizeof(double));//pdHist空间清0 
	//void *memset(void *s,int c,size_t n)将已开辟内存空间 s 的首 n 个字节的值设为值 c
	dDivider = 256.0 / (double)n;
	
	BYTE bGray;	// 临时变量,存储当前光标像素的灰度值
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			bGray = GetGray(j, i);
			pdHist[(int)(bGray / dDivider)]++;	// 指定的灰度区间自加
		}
	};

	UINT square = m_pBMIH->biWidth * m_pBMIH->biHeight;

	for (int k=0; k<n; k++) 
	{
		pdHist[k]=pdHist[k]/square;
	}
}



void CImg::GenHistrgb(double * pdHistr,double * pdHistg,double * pdHistb)
{
	// 计算分段因子
	double dDivider;
	COLORREF ref;
	BYTE r,g,b;
	int n=256;

	memset(pdHistr, 0, n * sizeof(double));//pdHist空间清0 
	memset(pdHistg, 0, n * sizeof(double));//pdHist空间清0 
	memset(pdHistb, 0, n * sizeof(double));//pdHist空间清0 
	//void *memset(void *s,int c,size_t n)将已开辟内存空间 s 的首 n 个字节的值设为值 c
	dDivider = 256.0 / (double)n;
	

	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			ref = GetPixel(j , i);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);
			pdHistr[(int)(r / dDivider)]++;	// 指定的灰度区间自加
			pdHistg[(int)(g / dDivider)]++;	// 指定的灰度区间自加
			pdHistb[(int)(b / dDivider)]++;	// 指定的灰度区间自加
		}
	};

	UINT square = m_pBMIH->biWidth * m_pBMIH->biHeight;

	for (int k=0; k<n; k++) 
	{
		pdHistr[k]=pdHistr[k]/square;
		pdHistg[k]=pdHistg[k]/square;
		pdHistb[k]=pdHistb[k]/square;
	}
}
          
int CImg::DetectThreshold(int iter)
{

	int nThreshold;
	
	// 直方图数组
	int nHistogram[256] = { 0 };
	int i, j;
	
	BYTE bt;
	int nDiffRet;
	int nMin = 255;
	int nMax = 0;
	int width=GetWidthPixel();
	int height=GetHeight();
	int nMaxIter=100;
//	BYTE gray;

	// 扫描图像,计算出最大、最小灰度和直方图
	for(j = 0; j < height; j ++)
	{
		for(i=0; i<width; i++)
		{
//			gray = pImg->GetGray(i,j);
			bt = GetGray(i,j);
			
			if(bt < nMin)
				nMin = bt;
			if(bt > nMax)
				nMax = bt;
			
			nHistogram[bt] ++;
			
		}
	}
	
	int nTotalGray = 0; //灰度值的和
	int nTotalPixel = 0; //像素数的和
	int nNewThreshold = (nMax + nMin)/2; //初始阈值
	
	nDiffRet = nMax - nMin;

	if (nMax == nMin)
		nThreshold = nNewThreshold;

	else
	{
		nThreshold = 0;
		
		// 迭代开始,直到迭代次数达到100或新阈值与上一轮得到的阈值相等，迭代结束
		for(int nIterationTimes = 0; nThreshold != nNewThreshold && nIterationTimes < nMaxIter; nIterationTimes ++)
		{
			nThreshold = nNewThreshold;
			nTotalGray = 0;
			nTotalPixel = 0;

			//计算图像中小于当前阈值部分的平均灰度
			for(i=nMin; i<nThreshold; i++)
			{
				nTotalGray += nHistogram[i]*i;
				nTotalPixel += nHistogram[i];
			}
			int nMean1GrayValue = nTotalGray/nTotalPixel;


			nTotalGray = 0;
			nTotalPixel = 0;
			
			//计算图像中大于当前阈值部分的平均灰度
			for(i=nThreshold + 1; i<=nMax; i++)
			{
				nTotalGray += nHistogram[i]*i;
				nTotalPixel += nHistogram[i];
			}
			int nMean2GrayValue = nTotalGray/nTotalPixel;
	
			nNewThreshold = (nMean1GrayValue + nMean2GrayValue)/2; //计算出新的阈值
			nDiffRet = abs(nMean1GrayValue - nMean2GrayValue);
		}




	}
	return nThreshold;
}


void CImg::Threshold(CImg* pImg, int thres)
{
	int width=GetWidthPixel();
	int height=GetHeight();
	BYTE bt;

	for(int j = 0; j < height; j ++)
	{
		for(int i=0; i<width; i++)
		{
//			gray = pImg->GetGray(i,j);
			bt = GetGray(i,j);
			
			if(bt < thres)
				bt=0;
			else
				bt = 255;
			
			pImg->SetPixel(i,j,RGB(bt,bt,bt));
		}
	}

}
