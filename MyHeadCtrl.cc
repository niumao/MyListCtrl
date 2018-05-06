// MyHeadCtrl.cpp : implementation file

#include "stdafx.h"
#include "MyHeadCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyHeadCtrl

CMyHeadCtrl::CMyHeadCtrl()
{
	m_nGradient = 0;	// 默认不绘制立体列头
	m_fHeight = 1;	// 默认列头高度是一倍数
	m_pFont = NULL;
	m_nDown = -1;
	m_nUp = -1;
	m_offset = 1;

	m_nFont = 0;
	m_nFontSize = 0;
	m_pColFont = NULL;
}

CMyHeadCtrl::~CMyHeadCtrl()
{
	if (m_pFont != NULL)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	if (m_pColFont != NULL)
	{
		delete[] m_pColFont;
		m_pColFont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMyHeadCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CMyHeadCtrl)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
//	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
//	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyHeadCtrl message handlers

void CMyHeadCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CHeaderCtrl::OnPaint()
	int nItem;
	CBrush _brush;
	nItem = GetItemCount(); // 得到有几个单元
	for (DWORD i = 0; i < (DWORD)nItem; i++) 
	{ 
		CRect tRect;
		int R, G, B;
		GetItemRect(i, &tRect); // 得到Item的尺寸
		HEADCELL *pHead = FindHeadCell(i);
		if (pHead)
		{
			R = GetRValue(pHead->rgbBack);
			G = GetGValue(pHead->rgbBack);
			B = GetBValue(pHead->rgbBack);
		}
		else
		{
			R = 212;
			G = 208;
			B = 200;
		}

		CRect nRect(tRect); // 拷贝尺寸到新的容器中 
		nRect.left++; // 留出分割线的地方 
		// 绘制立体背景
		if (m_nGradient == -1)
		{
			for (int j = tRect.top; j <= tRect.bottom - 2; j++) 
			{ 
				nRect.top = j;
				nRect.bottom = nRect.top + 1;
				CBrush _brush; 
				_brush.CreateSolidBrush(RGB(R,G,B)); // 创建画刷 
				dc.FillRect(&nRect, &_brush); // 填充背景 
				_brush.DeleteObject(); // 释放画刷 
				if (R < 0)	R = 0;
				if (G < 0)	G = 0;
				if (B < 0)	B = 0;
			}
		}
		else if (m_nGradient > 0)
		{
			for (int j = tRect.top; j <= tRect.bottom; j++) 
			{ 
				nRect.top = j;
				nRect.bottom = nRect.top + 1;
				CBrush _brush; 
				_brush.CreateSolidBrush(RGB(R,G,B)); // 创建画刷 
				dc.FillRect(&nRect, &_brush); // 填充背景 
				_brush.DeleteObject(); // 释放画刷 
				R -= m_nGradient;
				G -= m_nGradient;
				B -= m_nGradient;
				if (R < 0)
					R = 0;
				if (G < 0)
					G = 0;
				if (B < 0)
					B = 0;
			}
		}
		else
		{
			CRect rect = nRect;
			CBrush brush;
			brush.CreateSolidBrush(RGB(R,G,B));
			rect.left -= 1;
			rect.right += 1;
			rect.top -= 1;
			dc.FillRect(&rect, &brush);
			dc.DrawEdge(&rect, BDR_RAISEDINNER | EDGE_RAISED, BF_RECT);
		}
		////////////////////////////////////////////////////////////
		if (i == (DWORD)m_nDown)
		{
			CRect rect = nRect;
			CBrush brush;
			brush.CreateSolidBrush(RGB(R,G,B));
			rect.left -= 1;
			rect.right += 1;
			rect.top -= 1;
			dc.FillRect(&rect, &brush);
			dc.DrawEdge(&rect, BDR_SUNKENOUTER | EDGE_SUNKEN, BF_RECT);

			m_nDown = -1;
		}

		if (i == (DWORD)m_nUp)
		{
			CRect rect = nRect;
			CBrush brush;
			brush.CreateSolidBrush(RGB(R,G,B));	
			rect.left -= 1;
			rect.right += 1;
			rect.top -= 1;
			dc.FillRect(&rect, &brush);
			dc.DrawEdge(&rect, BDR_RAISEDINNER | EDGE_RAISED, BF_RECT);

			m_nUp = -1;
		}

		//绘制文本颜色和文本
		dc.SetBkMode(TRANSPARENT);
		CFont *pOldFont = NULL;

		if (pHead)
			dc.SetTextColor(pHead->rgbText);
		else
			dc.SetTextColor(RGB(255,255,255));

		if (m_pFont != NULL)
		{
			pOldFont = dc.SelectObject(m_pFont);
		}

		for (int k = 0; k < m_nFont; k++)
		{
			if ((DWORD)m_pColFont[k].nCol == i)
			{
				if (pOldFont != NULL)
					dc.SelectObject(pOldFont);
				pOldFont = dc.SelectObject(&m_pColFont[k].font);
				break;
			}
		}

		UINT nFormat;
		char cFormat = 1; // 默认
		if (pHead)
		{
			cFormat = pHead->cFormat;
		}
		///////////////////////////
		if (cFormat == 0)
		{
			nFormat = DT_LEFT;
			tRect.left += 3;
		}
		else if (cFormat == 1)
		{
			nFormat = DT_CENTER;
		}
		else if (cFormat == 2)
		{
			nFormat = DT_RIGHT;
			tRect.right -= 3;
		}
		TEXTMETRIC metric;
		dc.GetTextMetrics(&metric);
		int ofst = 0;
		int nBS = 1;
		for (int kkk = 0; kkk < pHead->strHead.GetLength(); kkk++)
		{
			int tt = pHead->strHead.Find("\r", kkk);
			if (tt >= 0)
				nBS++;
			else
				break;
			kkk += tt;
		}
		ofst = tRect.Height() - metric.tmHeight * nBS;
		tRect.OffsetRect(0, ofst / 2);
		if (pHead)
			dc.DrawText(pHead->strHead, &tRect, nFormat);
		else
			dc.DrawText("", &tRect, nFormat);
		if (pOldFont != NULL)
			dc.SelectObject(pOldFont);
	} 
	// 画头部剩余部分
	CRect rtRect;
	CRect clientRect;
	GetItemRect(nItem - 1, rtRect);
	GetClientRect(clientRect);
	rtRect.left = rtRect.right + 1;
	rtRect.right = clientRect.right;
	int R = 198,G = 198,B = 198;
	CRect nRect(rtRect);
	// 绘制立体背景 
	for (int j = rtRect.top; j <= rtRect.bottom; j++) 
	{ 
		nRect.bottom = nRect.top + 1; 
		CBrush _brush; 
		_brush.CreateSolidBrush(RGB(R,G,B)); // 创建画刷 
		dc.FillRect(&nRect, &_brush); // 填充背景 
		_brush.DeleteObject(); // 释放画刷 
		R -= m_nGradient;
		G -= m_nGradient;
		B -= m_nGradient;
		if (R < 0)	R = 0;
		if (G < 0)	G = 0;
		if (B < 0)	B = 0;
		nRect.top = nRect.bottom; 
	} 
	// Do not call CHeaderCtrl::OnPaint() for painting messages
}

void CMyHeadCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CHeaderCtrl::OnActivate(nState, pWndOther, bMinimized);
	// TODO: Add your message handler code here
}

LRESULT CMyHeadCtrl::OnLayout(WPARAM wParam,LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *(HD_LAYOUT *)lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 

	// 表头高度为原来1.5倍，如果要动态修改表头高度的话，将1.5设成一个全局变量 
	int nHeight = (int)(pwpos->cy * m_fHeight);
	pwpos->cy = nHeight; 
	prc->top = nHeight; 
	return lResult; 
}

HEADCELL *CMyHeadCtrl::FindHeadCell(int col)
{
	for (int i = 0; i < m_HeadCell.GetSize(); i++)
	{
		if (m_HeadCell[i].nCol == col)
		{
			return &m_HeadCell[i];
		}
	}
	return NULL;
}

//void CMyHeadCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
//{
//	// TODO: Add your message handler code here and/or call default
//	int nItem = GetItemCount(); // 得到有几个单元
//	CRect rect;
//	for (int i = 0; i < nItem; i++)
//	{
//		GetItemRect(i, &rect);
//		if (rect.PtInRect(point))
//		{
//			m_nDown = i;
//			break;
//		}
//	}
//	CHeaderCtrl::OnLButtonDown(nFlags, point);
//}

//void CMyHeadCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
//{
//	// TODO: Add your message handler code here and/or call default
//	int nItem = GetItemCount(); // 得到有几个单元
//	CRect rect;
//	for (int i = 0; i < nItem; i++)
//	{
//		GetItemRect(i, &rect);
//		if (rect.PtInRect(point))
//		{
//			m_nUp = i;
//			break;
//		}
//	}
//	CHeaderCtrl::OnLButtonUp(nFlags, point);
//}

void CMyHeadCtrl::SetColFont(int col, int nSize, BOOL bBold, BOOL bItalic, BOOL bUnderLine, LPCTSTR pszName)
{
	for (int i = 0; i < m_nFont; i++)
	{
		if (m_pColFont[i].nCol == col)
		{
			m_pColFont[i].font.DeleteObject();
			m_pColFont[i].font.CreateFont(-nSize, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, bUnderLine, 0, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, pszName);
			return;
		}
	}
	///////////////////////////////////////////
	if (m_nFont >= m_nFontSize)
	{
		HEADFONT *pTmp1, *pTmp2;
		pTmp1 = m_pColFont;
		m_nFontSize += 10;
		pTmp2 = new HEADFONT[m_nFontSize];
		if (m_pColFont != NULL)
			memcpy(pTmp2, m_pColFont, sizeof(HEADFONT)*m_nFont);
		m_pColFont = pTmp2;
		if (m_pColFont != NULL)
			delete[] pTmp1;
	}
	m_pColFont[m_nFont].nCol = col;
	if (m_pColFont != NULL)
		m_pColFont[m_nFont].font.DeleteObject();
	m_pColFont[m_nFont].font.CreateFont(-nSize, 0, 0, 0,bBold ? FW_BOLD : FW_NORMAL, bItalic, bUnderLine, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, pszName);
	//////////////////////////////
	m_nFont++;
}
