#include "pch.h"
#include "CColorListBox.h"
#include "OICQ.h"
#include "map"
IMPLEMENT_DYNAMIC(CColorListBox, CListBox)
CColorListBox::CColorListBox()
{
	m_NormalColor = RGB(255, 255, 255); //白色
	m_DownColor = RGB(39, 149, 241); //设置为默认天蓝色
	m_EdgeColor = GetSysColor(COLOR_BTNFACE); //边框颜色为按钮灰
	m_EdgeSpace = 5; //上下边距默认5
}


CColorListBox::~CColorListBox()
{
}


BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
	ON_WM_NCCALCSIZE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//设置按钮的按下弹起等状态颜色
void CColorListBox::SetSkin(COLORREF normal, COLORREF down, COLORREF edge, int edgespace)
{
	m_NormalColor = normal;
	m_DownColor = down;
	m_EdgeColor = edge;
	m_EdgeSpace = edgespace;
	return;
}


//设置单个条目高度
void CColorListBox::SetItemHeight(int cy)
{
	if (cy > 0)
		m_ItemHeigh = cy;
	return;
}


void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect = lpDrawItemStruct->rcItem;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CString strText;
	int nIndex = lpDrawItemStruct->itemID;
	if (nIndex == -1) {
		return;
	}
	else GetText(nIndex, strText);

	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);

	// Save these value to restore them when done drawing.  
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	// If this item is selected, set the background color   
	// and the text color to appropriate values. Also, erase  
	// rect by filling it with the background color.  

	//获取当前item 窗体位置
	CRgn rgn1;
	RECT r = lpDrawItemStruct->rcItem;
	RECT tr = { r.left, r.top + m_EdgeSpace, r.right, r.bottom - m_EdgeSpace };
	rgn1.CreateRectRgnIndirect(&tr);

	//窗口被选中 或者为活动
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));//字体颜色白色
		dc.SetBkColor(m_DownColor);//::GetSysColor(COLOR_HIGHLIGHT)

		CBrush brush1(m_DownColor);
		dc.FillRgn(&rgn1, &brush1);
		brush1.DeleteObject();

		//	SetCurSel(lpDrawItemStruct->itemID + 2);

			//dc.FillSolidRect(&lpDrawItemStruct->rcItem,::GetSysColor(COLOR_HIGHLIGHT));
	}
	else//窗口未被选中
	{
		//内嵌窗体颜色绘制
		CBrush brush1(m_NormalColor);
		dc.FillRgn(&rgn1, &brush1);

		//内嵌窗体边框绘制
		CBrush brush2(m_EdgeColor);
		dc.FrameRgn(&rgn1, &brush2, 2, 2);

		brush1.DeleteObject();
		brush2.DeleteObject();
		/*if (lpDrawItemStruct->itemID % 2)
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(128, 128, 128));
		else
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 128, 255));*/
	}

	lpDrawItemStruct->rcItem.left += 5;

	// Draw the text.  
	//dc.DrawText(lpszText, strlen(buf), &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dc.DrawText(strText, &tr, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	// Reset the background color and the text color back to their  
	// original values.  
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	dc.Detach();
	rgn1.DeleteObject();
	return;
}

void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
	LPCWSTR lpszText = (LPCWSTR)lpMeasureItemStruct->itemData;
	ASSERT(lpszText != NULL);
	CSize sz;
	CDC* pDC = GetDC();

	sz = pDC->GetTextExtent(lpszText);

	ReleaseDC(pDC);

	int height = 2 * sz.cy + 10;
	lpMeasureItemStruct->itemHeight = (m_ItemHeigh > 0) ? m_ItemHeigh : height;

	return;

}

//实现滚轮滚动 但不显示滚动条
void CColorListBox::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	ShowScrollBar(SB_BOTH, FALSE);
	CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
}



void CColorListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int nIndex = GetCurSel();//双击了哪个 Item
	int nCount = GetCount();//得到总共的 Item 数
	if (nIndex >= 0 && nIndex < nCount)
	{
		CString tmp_name;
		CString item;
		CString state;
		this->GetText(nIndex, item);
		int index = item.Find(_T(" "));
		tmp_name = item.Left(index);
		state = item.Right(sizeof(CString(" [离线]")));

		if (theApp.dlgs.find(tmp_name) != theApp.dlgs.end()) {
			theApp.dlgs[tmp_name]->ShowWindow(SW_SHOWNORMAL);
		}
		else {//没找到
			theApp.dlgs[tmp_name] = new CChatDlg(tmp_name, this);
			theApp.dlgs[tmp_name]->Create(IDD_CHAT_DIALOG);
			theApp.dlgs[tmp_name]->ShowWindow(SW_SHOWNORMAL);
			state == CString("[离线]") ? theApp.dlgs[tmp_name]->state = 0: theApp.dlgs[tmp_name]->state = 1;
			theApp.dlgs[tmp_name]->SetWindowTextW(item);
		}
	}

	CListBox::OnLButtonDblClk(nFlags, point);
}
