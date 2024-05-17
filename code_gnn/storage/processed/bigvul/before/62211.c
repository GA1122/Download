INT_PTR CALLBACK TooltipCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPNMTTDISPINFOW lpnmtdi;
	int i = MAX_TOOLTIPS;

	for (i=0; i<MAX_TOOLTIPS; i++) {
		if (ttlist[i].hTip == hDlg) break;
	}
	if (i == MAX_TOOLTIPS)
		return (INT_PTR)FALSE;

	switch (message) {
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TTN_GETDISPINFOW:
			lpnmtdi = (LPNMTTDISPINFOW)lParam;
			lpnmtdi->lpszText = ttlist[i].wstring;
			SendMessage(hDlg, TTM_SETMAXTIPWIDTH, 0, 300);
			return (INT_PTR)TRUE;
		}
		break;
	}
#ifdef _DEBUG
	if (GetCurrentThreadId() != MainThreadId)
		uprintf("Warning: Tooltip callback is being called from wrong thread");
#endif
	return CallWindowProc(ttlist[i].original_proc, hDlg, message, wParam, lParam);
}