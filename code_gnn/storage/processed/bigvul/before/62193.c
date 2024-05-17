INT_PTR CALLBACK LicenseCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	long style;
	HWND hLicense;
	switch (message) {
	case WM_INITDIALOG:
		hLicense = GetDlgItem(hDlg, IDC_LICENSE_TEXT);
		apply_localization(IDD_LICENSE, hDlg);
		CenterDialog(hDlg);
		style = GetWindowLong(hLicense, GWL_EXSTYLE);
		style &= ~(WS_EX_RTLREADING | WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR);
		SetWindowLong(hLicense, GWL_EXSTYLE, style);
		style = GetWindowLong(hLicense, GWL_STYLE);
		style &= ~(ES_RIGHT);
		SetWindowLong(hLicense, GWL_STYLE, style);
		SetDlgItemTextA(hDlg, IDC_LICENSE_TEXT, gplv3);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			reset_localization(IDD_LICENSE);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}