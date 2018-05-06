#if !defined(AFX_MYLISTCTRL1_H__6F14775C_CA2F_464E_84E6_D400E924309E__INCLUDED_)
#define AFX_MYLISTCTRL1_H__6F14775C_CA2F_464E_84E6_D400E924309E__INCLUDED_

#include "MyHeadCtrl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl1.h : header file
//
#define CELL_MAXLEN 256

#define IDC_EDT_CTRL 5101
#define IDC_CMB_CTRL 5102
#define IDC_CHECK_CTRL 5103

#define CTRL_TYPE_EDIT	111
#define CTRL_TYPE_COMBO	112
#define CTRL_TYPE_CHECK	113

#define DATA_TYPE_CHAR		100 //(1字节)
#define DATA_TYPE_INT		101	//(2字节) WORD == unsigned short
#define DATA_TYPE_FLOAT		102 //(4字节)
#define DATA_TYPE_STRING	103
#define DATA_TYPE_UCHAR		104 //(1字节)
#define DATA_TYPE_LONG		105	//(4字节) DWORD == unsigned long
#define DATA_TYPE_DOUBLE	106 //(8字节)


typedef void(*COMBOKILLFOCUS) (CDialog* pDlg, int nCol);
typedef void(*ITEMDBLCLK) (int nItem,int nSubItem,void* pVoid);
/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window
typedef struct _CELLVALUE
{
	DWORD dwRow;//
	DWORD dwCol;//
	UINT nIDResource;//picture
	unsigned char cCtrlType;//111-IDC_EDIT,112-IDC_COMBO
	unsigned char cDataType;//100-char,101-int,102-float,103-char*
	WORD nStrMaxLen;
	char strPrevItem[CELL_MAXLEN];//这个ITEM前一个的值
	unsigned char bItemChange;
	void* pValue;	//存储的是对应值的指针!
	unsigned char nComboStr;
	CString* pComboStr;
}CELLVALUE;

typedef struct _stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
}stColor;

typedef struct _stFont
{
	_stFont(void){font.CreateFont(12,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,\
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"宋体");};
	~_stFont(){font.DeleteObject();};
	int nRow;
	int nCol;
	CFont font;
}stFont;

typedef struct _CELLSTYLE
{
	DWORD dwRow;//
	DWORD dwCol;//
	COLORREF rgbBk;
	COLORREF rgbText;
	char cFormat;//0-左对齐，1-中间对齐，2-右对齐
	//字体
	short nFontSize;
	bool bFontBold;
	bool bFontItalic;
	bool bFontUnderLine;
	char strFontName[32];
}CELLSTYLE;
/*********************************************
* CExArray扩展类:功能类似于CArray
* 用处：管理CELLVALUE结构指针
* 功能：添加、删除、获取结构（CELLVALUE）指针
**********************************************/
template<class T>
class CExArray
{
public:
	CExArray(int nSize=10){m_dwSize=10;m_dwCurCount=0;m_pVal=NULL;m_pVal=new T[nSize];};
	virtual ~CExArray(){if(m_pVal!=NULL){delete[] m_pVal;m_pVal=NULL;}};
	//////////////////////
public:
	void Add(T& cell);
	BOOL GetAt(DWORD dwRow,DWORD dwCol,T& cell);
	BOOL GetAt(DWORD dwIndex,T& cell);
	T* GetAt(DWORD dwRow,DWORD dwCol);
	T* GetAt(DWORD dwIndex);
	void DelAt(DWORD dwRow,DWORD dwCOl);
	void DelAll(void);
	int  GetCount(void);
private:
	T *m_pVal;
	DWORD m_dwSize;
	DWORD m_dwCurCount;
};

/***************************************************************************************
* 类名称：	CMyListCtrl ：public CListCtrl
* 类说明：	CMyListCtrl类是继承自CListCtrl类，对CListCtrl类进行重绘和扩长新功能
* 功能：	可以设置列表的行高大小  
*			可以设置列表头的行高大小
*			可以设置单元格的文本颜色、背景颜色、字体
*			可以设置列头每一列的文本颜色、背景颜色、字体
*			可以通过鼠标双击某个单元格显示控件（CEdit、CComboBox）对该单元格中内容修改
*			可以再单元格中显示位图资源
***************************************************************************************/
class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:
// Operations



//
public:
	//afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMDBClick(NMHDR *pNMHDR, LRESULT *pResult);



public:
	//0-char,1-int,2-float,3-char* ; 0-IDC_EDIT,1-IDC_CMB,注意：pData,pComboStr只是存储他的指针!
	void Attach(DWORD dwItem,DWORD dwSubItem,void* pData,unsigned char cDataType=0,unsigned char cCtrlType=0,\
		 int nComboStr=0,CString* pComboStr=NULL);

	//设置某一个单元格中最大可写的字符串
	void SetStrMaxLen(int nRow,int nCol,int nMaxLen=256);

	//获取ItemValue Change的Num
	int GetItemChangeNum(void);

	//在调用GetNextItemChange之前先调用这个函数一次既可以了
	bool GetFirstItemChange(int& row,int& col);

	//获取ItemValue Change的行号和列号
	bool GetNextItemChange(int& row,int& col);
	/////////////////////////////////////////////////////////////
	// 设置表头高度
	void SetHeadHeight(float Height);

	// Gradient - 渐变系数，立体背景用,不用渐变设为0
	void SetHeadBKColor(COLORREF rgb,int Gradient,int col=-1);
	/////////////////////////////////////////////////////////////

	//设置行颜色
	void SetRowColor(int row,COLORREF color);

	//取消设置的行颜色
	void UnSetRowColor(int row);

	//设置列颜色
	void SetColColor(int col,COLORREF color); 

	//设置单元格颜色
	void SetItemColor(int row,int col,COLORREF color);

	//设置行文本颜色
	void SetRowTextColor(int row,COLORREF color);

	//取消设置的行文本颜色
	void UnSetRowTextColor(int row);

	//设置列文本颜色
	void SetColTextColor(int col,COLORREF color);

	//设置单元格文本颜色
	void SetItemTextColor(int row,int col,COLORREF color);

	//设置行高
	void SetRowHeigt(int nHeight);

	//设置头部字体
	void SetHeadFont(int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="宋体");

	//设置头部字体颜色
	void SetHeadTextColor(COLORREF color,int col=-1);

	//////////////////////////////////////////////////////////////
	//设置整个列表的字体
	void SetFont(int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="宋体");

	//设置一列的字体
	void SetColFont(int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="宋体");

	//设置某一个单元格中的字体
	void SetItemFont(int row,int col,int nSize,BOOL bBold,BOOL bItalic,BOOL bUnderLine,LPCTSTR pszName="宋体");

	//设置列头某一列的字体
	void SetHeadColFont(int col,int nFontSize,BOOL Bold,BOOL Italic,BOOL UnderLine,LPCTSTR pszFontName="宋体");

	///////////////////////////////////////////////////////////////

	//查找某一列背景颜色
	int FindColColor(int col ,COLORREF &color);

	//查找某一行背景颜色
	int FindRowColor(int row ,COLORREF &color);

	//查找某一个单元格背景颜色
	int FindItemColor(int col,int row,COLORREF &color);

	//查找某一行文本颜色
	int FindRowTextColor(int row,COLORREF &color);

	//查找某一列文本颜色
	int FindColTextColor(int col,COLORREF &color);

	//查找某一单元格文本颜色
	int FindItemTextColor(int col,int row,COLORREF &color);

	//查找某一列的字体
	int FindColFont(int col);

	//查找某一个单元格的字体
	int FindItemFont(int row,int col);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	//插入一行,第一列写入Int值
	int	 InsertItemInt(int nItem,int val);

	//在指定单元格写入Int值
	BOOL SetItemInt(int nItem,int nSubItem,int val);

	//在指定单元格写入Float值
	BOOL SetItemFloat(int nItem,int nSubItem,float val);

	//获取指定单元格的值转化为Int类型
	int	 GetItemInt(int nItem,int nSubItem);
	
	//获取指定单元格的值转化为Float类型
	float GetItemFloat(int nItem,int nSubItem);
	//////////////////////////////////////////////////////////////////////////
	//清空列表
	void Clear(void);

	//清空列表头
	void ClearHead(void);

	//设置回调函数（KillComboBox的回调）
	void SetCallbackFunc(COMBOKILLFOCUS pFunc,CDialog* pDlg=NULL);

	// 单元格左键双击事件
	void SetItemDblClkFunc(ITEMDBLCLK pFunc,int nColItem,void* pParam);

	//获取当前选中的行
	int GetCurSel(void);

	//获取上一次选中的行
	int GetPrevSel(void);

	//设置当前选中的行
	void SetCurSel(int nItem);

	//设置选中一列的模式
	void SetColSelectMode(bool bColSelect=TRUE);

	//设置选中一行的模式
	void SetLineSelectMode(bool bLineSelect=TRUE,bool bLineSelectBkColor=TRUE);

	//COMBOBOX控件Value Change 时用于更新
	void UpdateCombo(int nCol,CString* pComboStr,int nComboStr);

	//设置单元中图片
	void SetItemImage(int row,int col,UINT nIDResource);

	//取消单元中的图片
	void UnSetItemImage(int row,int col);

	//判断单元格中是否有图片
	BOOL IsItemImage(int row,int col);

	//更新单元格中的文本串
	void Update(int row,int col,LPCTSTR pszTxt);

	//获取单元格的Rect
	void GetCellRect(int nItem,int nSubItem,CRect& rect);

	//插入一列(Format=DT_LEFT、DT_CENTER、DT_RIGHT)
	int InsertColumn(int col,LPCTSTR pszHead,UINT Format,int nHeadWidth);

	//插入所有列
	void InsertColumn(int num,CString* pHead,UINT Format,int* pHeadWidth);
	//当前选中的行下移
	void CMyListCtrl::OnSelectedDown();
	//当前选中的行上移
	void CMyListCtrl::OnSelectedUp();

	//响应的消息
	virtual ~CMyListCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//私有函数
private:
	void UpdateCharData(CELLVALUE* pcv,CString& strItem);
	void UpdateUCharData(CELLVALUE* pcv,CString& strItem);
	void UpdateStringData(CELLVALUE* pcv,CString& strItem);
	void UpdateIntData(CELLVALUE* pcv,CString& strItem);
	void UpdateLongData(CELLVALUE* pcv,CString& strItem);
	void UpdateFloatData(CELLVALUE* pcv,CString& strItem);
	void UpdateDoubleData(CELLVALUE* pcv,CString& strItem);
	// Generated message map functions
	//afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnSetfocusEdit();
	afx_msg void OnKillfocusEdit();
	afx_msg void OnSetfocusCombo();
	afx_msg void OnKillfocusCombo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	CRichEditCtrl* m_pEdit;

	//类的私有成员变量
private:
	int m_nCurSelCol;//当前选中的列
	int m_nPrevSelected;//上一次选中的行
	int m_nCurSelected;//当前选中的行
	bool m_bLineSelected;
	bool m_bLineSelectedMode;//一行选中模式
	int m_nColSelected;
	bool m_bColSelectedMode;//一列选中模式
	bool m_bLineSelectBkColor;//行选中模式时，选中一行是否需要背景色
	//Private Function
	BOOL CreateCtrl(CELLVALUE* pCV);
	///////////////////////////////
	CMyHeadCtrl m_Head;
	CFont *m_pFont;
	///////////////////////////////
	//控件
	//CRichEditCtrl* m_pEdit;
	CComboBox* m_pCombo;
	////////////////////////////

	////////////////////////////
	stFont* m_pItemFont;	//保存Item字体
	int m_nFont,m_nFontSize;
	////////////////////////////
	CExArray<CELLVALUE> m_val;
//	CExArray<CELLSTYLE> m_stl;
	CFont *m_pCmbFont;
	/////////////////
	CArray<stColor,stColor&> m_ptrListRow;		//保存行颜色
	CArray<stColor,stColor&> m_ptrListCol;		//保存列颜色
	CArray<stColor,stColor&> m_ptrListItem;		//保存Item颜色表
	CArray<stColor,stColor&> m_colTextColor;	//保存列字体颜色
	CArray<stColor,stColor&> m_rowTextColor;	//保存行字体颜色
	CArray<stColor,stColor&> m_ItemTextColor;	//保存单元格字体颜色
	/////////////////
	CELLVALUE* m_pCV;
	int m_nRowHeight;//行高!
	///////////////////////
	//CALLBACK FUNCTION
	COMBOKILLFOCUS m_pComboKillFocus;
	ITEMDBLCLK m_pItemDblClk;
	//CALLBACK FUNCTION PARAMS COMBOKILLFOCUS
	CDialog* m_pDlg;
	//CALLBACK FUNCTION PARAMS ITEMDLBCLK
	int m_nColItem;
	void* m_pVoid; // 返回的参数
	/////////////////////////////////////////////////
	//用于GetNextItemChange(int& row,int& col)函数
	int m_nCurItemChange;
	
};

/////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL1_H__6F14775C_CA2F_464E_84E6_D400E924309E__INCLUDED_)




// 控件颜色
#define HEADBKCOLOR		RGB(0,103,180)	 // 设置List表头背景色
#define HEADTEXTCOLOR	RGB(255,255,255) // 设置List表头文本颜色
#define ITEMBKCOLOR		RGB(255,255,255) // 设置List单元格背景色
#define ITEMTEXTCOLOR	RGB(0,0,0)		 // 设置List单元格文本颜色
#define FILLSOLIDRECT	RGB(51,153,255)	 // 设置List单元格选中时的背景色
#define RECTANGLE		RGB(51,153,255)	 // 设置List单元格选中时的矩形框颜色
#define TEXTCOLOR		RGB(255,255,255) // 设置List单元格选中时的文本颜色