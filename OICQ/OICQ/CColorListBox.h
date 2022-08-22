#pragma once
#include <afxwin.h>
class CColorListBox :public CListBox
{
	DECLARE_DYNAMIC(CColorListBox)
public:
	CColorListBox();
	~CColorListBox();

	//控件 常规  按下 边缘 颜色 以及上下边缘距离
	void SetSkin(COLORREF normal, COLORREF down, COLORREF edge = 0, int edgespace = 0);
	void SetItemHeight(int cy);//设置单个条目高度
private:
	COLORREF m_NormalColor;
	COLORREF m_DownColor;
	COLORREF m_EdgeColor;
	int m_EdgeSpace;
	int m_ItemHeigh;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


