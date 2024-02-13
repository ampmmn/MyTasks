#include "pch.h"
#include "DlgCtrlCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void AddColumn(CListCtrl* list, int index, LPCTSTR headerName, int width)
{
	LVCOLUMN lvc = {};
	lvc.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;

	CString strHeader(headerName);
	lvc.pszText = const_cast<LPTSTR>((LPCTSTR)strHeader);
	lvc.cx = width;
	lvc.fmt = LVCFMT_LEFT;
	list->InsertColumn(index, &lvc);
}


