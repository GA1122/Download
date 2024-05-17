INT_PTR CALLBACK NotificationCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT loc;
	int i;
	static LRESULT disabled[9] = { HTLEFT, HTRIGHT, HTTOP, HTBOTTOM, HTSIZE,
		HTTOPLEFT, HTTOPRIGHT, HTBOTTOMLEFT, HTBOTTOMRIGHT };
	static HBRUSH background_brush, separator_brush;
	NONCLIENTMETRICS ncm;
	HFONT hDlgFont;

	switch (message) {
	case WM_INITDIALOG:
		if (nWindowsVersion >= WINDOWS_VISTA) {	 
			ncm.cbSize = sizeof(ncm);
			#if defined(_MSC_VER) && (_MSC_VER >= 1500) && (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
			ncm.cbSize -= sizeof(ncm.iPaddedBorderWidth);
			#endif
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
			hDlgFont = CreateFontIndirect(&(ncm.lfMessageFont));
			SendMessage(hDlg, WM_SETFONT, (WPARAM)hDlgFont, MAKELPARAM(TRUE, 0));
			SendMessage(GetDlgItem(hDlg, IDC_NOTIFICATION_TEXT), WM_SETFONT, (WPARAM)hDlgFont, MAKELPARAM(TRUE, 0));
			SendMessage(GetDlgItem(hDlg, IDC_MORE_INFO), WM_SETFONT, (WPARAM)hDlgFont, MAKELPARAM(TRUE, 0));
			SendMessage(GetDlgItem(hDlg, IDYES), WM_SETFONT, (WPARAM)hDlgFont, MAKELPARAM(TRUE, 0));
			SendMessage(GetDlgItem(hDlg, IDNO), WM_SETFONT, (WPARAM)hDlgFont, MAKELPARAM(TRUE, 0));
		}

		apply_localization(IDD_NOTIFICATION, hDlg);
		background_brush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		separator_brush = CreateSolidBrush(GetSysColor(COLOR_3DLIGHT));
		SetTitleBarIcon(hDlg);
		CenterDialog(hDlg);
		if (Static_SetIcon(GetDlgItem(hDlg, IDC_NOTIFICATION_ICON), hMessageIcon) == 0) {
			uprintf("Could not set dialog icon\n");
		}
		if (szMessageTitle != NULL) {
			SetWindowTextU(hDlg, szMessageTitle);
		}
		if (!notification_is_question) {
			SetWindowTextU(GetDlgItem(hDlg, IDNO), lmprintf(MSG_006));
		} else {
			ShowWindow(GetDlgItem(hDlg, IDYES), SW_SHOW);
		}
		if ((notification_more_info != NULL) && (notification_more_info->callback != NULL)) {
			ShowWindow(GetDlgItem(hDlg, IDC_MORE_INFO), SW_SHOW);
		}
		if (szMessageText != NULL) {
			SetWindowTextU(GetDlgItem(hDlg, IDC_NOTIFICATION_TEXT), szMessageText);
		}
		return (INT_PTR)TRUE;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);
		if ((HWND)lParam == GetDlgItem(hDlg, IDC_NOTIFICATION_LINE)) {
			return (INT_PTR)separator_brush;
		}
		return (INT_PTR)background_brush;
	case WM_NCHITTEST:
		loc = DefWindowProc(hDlg, message, wParam, lParam);
		for(i = 0; i < 9; i++) {
			if (loc == disabled[i]) {
				return (INT_PTR)TRUE;
			}
		}
		return (INT_PTR)FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
		case IDYES:
		case IDNO:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDC_MORE_INFO:
			if (notification_more_info != NULL)
				MyDialogBox(hMainInstance, notification_more_info->id, hDlg, notification_more_info->callback);
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}