INT_PTR CALLBACK AboutCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i, dy;
	const int edit_id[2] = {IDC_ABOUT_BLURB, IDC_ABOUT_COPYRIGHTS};
	char about_blurb[2048];
	const char* edit_text[2] = {about_blurb, additional_copyrights};
	HWND hEdit[2];
	TEXTRANGEW tr;
	ENLINK* enl;
	RECT rect;
	REQRESIZE* rsz;
	wchar_t wUrl[256];
	static BOOL resized_already = TRUE;

	switch (message) {
	case WM_INITDIALOG:
		resized_already = FALSE;
		apply_localization(IDD_ABOUTBOX, hDlg);
		SetTitleBarIcon(hDlg);
		CenterDialog(hDlg);
		if (settings_commcheck)
			ShowWindow(GetDlgItem(hDlg, IDC_ABOUT_UPDATES), SW_SHOW);
		static_sprintf(about_blurb, about_blurb_format, lmprintf(MSG_174|MSG_RTF),
			lmprintf(MSG_175|MSG_RTF, rufus_version[0], rufus_version[1], rufus_version[2]),
			right_to_left_mode?"Akeo \\\\ Pete Batard 2011-2017 © Copyright":"Copyright © 2011-2017 Pete Batard / Akeo",
			lmprintf(MSG_176|MSG_RTF), lmprintf(MSG_177|MSG_RTF), lmprintf(MSG_178|MSG_RTF));
		for (i=0; i<ARRAYSIZE(hEdit); i++) {
			hEdit[i] = GetDlgItem(hDlg, edit_id[i]);
			SendMessage(hEdit[i], EM_AUTOURLDETECT, 1, 0);
			 
			SendMessageA(hEdit[i], EM_SETTEXTEX, (WPARAM)&friggin_microsoft_unicode_amateurs, (LPARAM)edit_text[i]);
			SendMessage(hEdit[i], EM_SETSEL, -1, -1);
			SendMessage(hEdit[i], EM_SETEVENTMASK, 0, ENM_LINK|((i==0)?ENM_REQUESTRESIZE:0));
			SendMessage(hEdit[i], EM_SETBKGNDCOLOR, 0, (LPARAM)GetSysColor(COLOR_BTNFACE));
		}
		SendMessage(hEdit[1], EM_SETSEL, 0, 0);
		SendMessage(hEdit[0], EM_REQUESTRESIZE, 0, 0);
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case EN_REQUESTRESIZE:
			if (!resized_already) {
				resized_already = TRUE;
				GetWindowRect(GetDlgItem(hDlg, edit_id[0]), &rect);
				dy = rect.bottom - rect.top;
				rsz = (REQRESIZE *)lParam;
				dy -= rsz->rc.bottom - rsz->rc.top;
				ResizeMoveCtrl(hDlg, GetDlgItem(hDlg, edit_id[0]), 0, 0, 0, -dy, 1.0f);
				ResizeMoveCtrl(hDlg, GetDlgItem(hDlg, edit_id[1]), 0, -dy, 0, dy, 1.0f);
			}
			break;
		case EN_LINK:
			enl = (ENLINK*) lParam;
			if (enl->msg == WM_LBUTTONUP) {
				tr.lpstrText = wUrl;
				tr.chrg.cpMin = enl->chrg.cpMin;
				tr.chrg.cpMax = enl->chrg.cpMax;
				SendMessageW(enl->nmhdr.hwndFrom, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
				wUrl[ARRAYSIZE(wUrl)-1] = 0;
				ShellExecuteW(hDlg, L"open", wUrl, NULL, NULL, SW_SHOWNORMAL);
			}
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			reset_localization(IDD_ABOUTBOX);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDC_ABOUT_LICENSE:
			MyDialogBox(hMainInstance, IDD_LICENSE, hDlg, LicenseCallback);
			break;
		case IDC_ABOUT_UPDATES:
			MyDialogBox(hMainInstance, IDD_UPDATE_POLICY, hDlg, UpdateCallback);
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}