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
		CString t("ͼ���С��ͬ���޷��Ƚϣ�");
		MessageBox(0,t,0,MB_OK);
		return grayRet;
	}

	// ����ͼ�����
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
		CString t("ͼ���С��ͬ���޷��Ƚϣ�");
		MessageBox(0,t,0,MB_OK);
		return grayRet;
	}


	// ����ͼ��Ļ�
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

���ܣ�ͼ��λ��

������
	CImg ����

����ֵ:
	CImg ��Ӻ�� CImg ��ͼ�����
*******************/

CImg CImg::operator + (CImg gray)
{
	CImg grayRet; //����ͼ��
	grayRet = *this;
	BYTE dgray;
	//ȡ��ͼ��ĸߺͿ�
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;//ѭ������
	
	//����ɨ��ͼ��
	for(j=0; j<nHeight; j++)
	{
		for(i=0; i<nWidth; i++)
		{
			//��λ���
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

���ܣ�
	�ø����Ĵ�С���³�ʼ��CImg����

���ƣ�
	CImg��������Ѿ�������Ч��ͼ�����ݣ����򽫳���

������
	int nHeight
		���³�ʼ���ɵĿ���
	int nWidth
		���³�ʼ���ɵĸ߶�
����ֵ��
	��
***************************************************/
void CImg::ImResize(int nHeight, int nWidth)
{
	int i; //ѭ������
	//�ͷ�ͼ�����ݿռ�
	for(i=0; i<m_pBMIH->biHeight; i++)
	{
		delete[] m_lpData[i];
	}
	delete[] m_lpData;

	//������Ϣͷ�е���Ӧ����
	m_pBMIH->biHeight = nHeight; //���¸߶�
	m_pBMIH->biWidth = nWidth; //���¿���

	//���·������ݿռ�
	m_lpData = new LPBYTE [nHeight];
	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	for(i=0; i<nHeight; i++)
	{
		m_lpData[i] = new BYTE [nWidthBytes];
	}
}

/**************************************************
void CImg::InitPixels(BYTE color)
���ܣ�
	�ø�������ɫֵ��ʼ��ͼ�����������

���ƣ�
	ֻ��ʹ�ûҶ�ֵ�ṩ��ɫֵ����ֻ�ܳ�ʼ��Ϊĳ�ֻ�ɫ
������
	BYTE color��ָ����������ʼ��ͼ��ĻҶ�ֵ
����ֵ��
	��
***************************************************/
void CImg::InitPixels(BYTE color)
{
	//���ͼ��ߡ���
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;//�С���ѭ������

	//����ɨ��ͼ�����ζ�ÿ����������color�Ҷ�
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

���ܣ�
	��ָ����ͼ���ļ������ӵ�CImg������
���ƣ�
	ֻ�ܴ���λͼͼ��

������LPCTSTR lpcPathName
		�����ļ�������·��
����ֵ��
	BYTE���ͣ�TRUEΪ�ɹ���FALSEΪʧ��
***************************************************/
BOOL CImg::AttachFromFile(LPCTSTR lpcPathName)
{
	// ʹ��CFile����򻯲���
	CFile file;
	if(!file.Open(lpcPathName, CFile::modeRead|CFile::shareDenyWrite))
		return FALSE;

	BOOL bSuc = AttachFromFile(file);

	file.Close();
	return bSuc;
}

/**************************************************
BOOL CImg::AttachFromFile(CFile &file)

���ܣ�
	��ָ����ͼ���ļ������ӵ�CImg������
���ƣ�
	ֻ�ܴ���λͼͼ��

������
	CFile &file
		���򿪵�CFile����
����ֵ��
	BYTE���ͣ�TRUEΪ�ɹ���FALSEΪʧ��
***************************************************/

BOOL CImg::AttachFromFile(CFile &file)
{
	// �ļ�����
	LPBYTE  *lpData;
	// λͼ��Ϣͷ
	BITMAPINFOHEADER *pBMIH;
	// ��ɫ��ָ��
	LPVOID lpvColorTable = NULL;
	// ��ɫ����ɫ��Ŀ
	int nColorTableEntries;

	BITMAPFILEHEADER bmfHeader;

	// ��ȡ�ļ�ͷ
	if(!file.Read(&bmfHeader, sizeof(bmfHeader)))
		return FALSE;

	
	// ��鿪ͷ���ֽ��Ƿ�ΪBM
	if(bmfHeader.bfType != MAKEWORD('B', 'M'))
	{
		return FALSE;
	}

	// ��ȡ��Ϣͷ
	pBMIH = (BITMAPINFOHEADER*)new BYTE[bmfHeader.bfOffBits - sizeof(bmfHeader)];
	if(!file.Read(pBMIH, bmfHeader.bfOffBits - sizeof(bmfHeader)))
	{
		delete pBMIH;
		return FALSE;
	}

	// ��λ����ɫ��
	nColorTableEntries = 
		(bmfHeader.bfOffBits - sizeof(bmfHeader) - sizeof(BITMAPINFOHEADER))/sizeof(RGBQUAD);
	if(nColorTableEntries > 0)
	{
		lpvColorTable = pBMIH + 1;
	}

	pBMIH->biHeight = abs(pBMIH->biHeight);

	// ��ȡͼ������,WIDTHBYTES����������ÿ���ֽ���
	int nWidthBytes = WIDTHBYTES((pBMIH->biWidth)*pBMIH->biBitCount);

	// ����biHeight������ΪbiWidthBytes������,������������λͼ����
	lpData = new LPBYTE[(pBMIH->biHeight)];
	for(int i=0; i<(pBMIH->biHeight); i++)
	{
		lpData[i] = new BYTE[nWidthBytes];
		file.Read(lpData[i], nWidthBytes);
		
	}

	// ��������
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

���ܣ�
	��CImgʵ���е�ͼ�����ݱ��浽ָ����ͼ���ļ�
���ƣ�
	ֻ�ܴ���λͼͼ��

������
	CFile &file
		�����浽��CFile����
����ֵ��
	BYTE���ͣ�TRUEΪ�ɹ���FALSEΪʧ��
***************************************************/
BOOL CImg::SaveToFile(CFile &file)
{	
	// �ж��Ƿ���Ч
	if(!IsValidate())
		return FALSE;	
	
		
	// ����BITMAPFILEHEADER�ṹ
	BITMAPFILEHEADER bmfHeader = { 0 };
	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);


	bmfHeader.bfType = MAKEWORD('B', 'M');
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) 
				+ sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4;

	bmfHeader.bfSize = bmfHeader.bfOffBits + m_pBMIH->biHeight * nWidthBytes;
	
	// ���ļ���д������
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

���ܣ�
	�ڸ������豸�����Ļ����н�CImg�����д洢��ͼ����Ƶ���Ļ��

���ƣ�
	��

������
	CDC * pDC
		ָ�����豸�����Ļ�����ָ��
����ֵ��
	�������ͣ�TRUEΪ�ɹ���FALSEΪʧ��
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

���ܣ�
	�趨ָ������λ�����ص���ɫֵ

���ƣ�
	��

������
	int x, int y
		ָ�������غᡢ������ֵ
	COLORREF
		���趨��ָ��λ�õ���ɫֵ��RGB��ʽ����
����ֵ��
	��
***************************************************/
void CImg::SetPixel(int x, int y, COLORREF color)
{
	if(m_pBMIH->biBitCount == 8)			// 256ɫͼ
	{
		m_lpData[m_pBMIH->biHeight - y - 1][x] = GetRValue(color);
	}
	else if(m_pBMIH->biBitCount == 1)		// ��ɫͼ
	{
		BYTE Color = GetRValue(color);


		// ��0������
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			Color = !Color;

		y = m_pBMIH->biHeight - y - 1;
		
		// ��ɫ��
		if (Color == 0)  // ���ö�ӦλΪ0
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
		else // ��ɫ��
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
//	// ��ȡ����
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


// ����ͼ��ת�Ҷ�ͼ��
bool CImg::Index2Gray()
{
	int i;

	if (!IsIndexedImg()) return false;
	RGBQUAD *table = (RGBQUAD*)m_lpvColorTable;

	m_pBMIH->biBitCount = 8;

	// ������ɫ����
	for (i=0; i<GetHeight(); i++)
	{
		for (int j=0; j<GetWidthPixel(); j++)
		{
			RGBQUAD rgb = *(table+GetGray(j, i));
			BYTE gray = rgb.rgbBlue * 0.114 + rgb.rgbGreen * 0.587 + rgb.rgbRed * 0.299 + 0.5;
			SetPixel(j, i, RGB(gray, gray, gray));
		}
	}
	
	// ������ɫ��
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

���ܣ�ͼ��λ��

������
	CImg ����

����ֵ:
	CImg�� ������ CImg ��ͼ�����
*******************/
CImg CImg::operator - (CImg &gray)
{
	CImg grayRet; //����ͼ��
	grayRet = *this;

	//ȡ��ͼ��ĸߺͿ�
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j, color;

	//����ɨ��ͼ��
	for(j=0; j<nHeight; j++)
	{
		for(i=0; i<nWidth; i++)
		{
			//��λ���
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
	// ����ֶ�����
	double dDivider;

	memset(pdHist, 0, n * sizeof(double));//pdHist�ռ���0 
	//void *memset(void *s,int c,size_t n)���ѿ����ڴ�ռ� s ���� n ���ֽڵ�ֵ��Ϊֵ c
	dDivider = 256.0 / (double)n;
	
	BYTE bGray;	// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			bGray = GetGray(j, i);
			pdHist[(int)(bGray / dDivider)]++;	// ָ���ĻҶ������Լ�
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
	// ����ֶ�����
	double dDivider;
	COLORREF ref;
	BYTE r,g,b;
	int n=256;

	memset(pdHistr, 0, n * sizeof(double));//pdHist�ռ���0 
	memset(pdHistg, 0, n * sizeof(double));//pdHist�ռ���0 
	memset(pdHistb, 0, n * sizeof(double));//pdHist�ռ���0 
	//void *memset(void *s,int c,size_t n)���ѿ����ڴ�ռ� s ���� n ���ֽڵ�ֵ��Ϊֵ c
	dDivider = 256.0 / (double)n;
	

	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			ref = GetPixel(j , i);
			r = GetRValue(ref);
			g = GetGValue(ref);
			b = GetBValue(ref);
			pdHistr[(int)(r / dDivider)]++;	// ָ���ĻҶ������Լ�
			pdHistg[(int)(g / dDivider)]++;	// ָ���ĻҶ������Լ�
			pdHistb[(int)(b / dDivider)]++;	// ָ���ĻҶ������Լ�
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
	
	// ֱ��ͼ����
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

	// ɨ��ͼ��,����������С�ҶȺ�ֱ��ͼ
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