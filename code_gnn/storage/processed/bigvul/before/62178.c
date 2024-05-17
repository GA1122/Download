void CreateStatusBar(void)
{
	SIZE sz = {0, 0};
	RECT rect;
	LONG x, y, width, height;
	int edge[3];
	TBBUTTON tbbStatusToolbarButtons[1];
	TBBUTTONINFO tbi;
	HFONT hFont;
	HDC hDC;

	hStatus = CreateWindowExW(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_TOOLTIPS | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMainDialog,
		(HMENU)IDC_STATUS, hMainInstance, NULL);

	GetClientRect(hStatus, &rect);
	height = rect.bottom;

	hFont = CreateFontA(-MulDiv(10, GetDeviceCaps(GetDC(hMainDialog), LOGPIXELSY), 72),
		0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		0, 0, PROOF_QUALITY, 0, (nWindowsVersion >= WINDOWS_VISTA)?"Segoe UI":"Arial Unicode MS");

	hDC = GetDC(hMainDialog);
	SelectObject(hDC, hFont);
	GetTextExtentPoint32W(hDC, L"#", 1, &sz);
	if (hDC != NULL)
		ReleaseDC(hMainDialog, hDC);

	GetClientRect(hMainDialog, &rect);
	edge[1] = rect.right - (int)(SB_TIMER_SECTION_SIZE * fScale);
	edge[0] = edge[1] - (8 + sz.cx + 8 + 1);  
	edge[2] = rect.right;
	SendMessage(hStatus, SB_SETPARTS, (WPARAM)ARRAYSIZE(edge), (LPARAM)&edge);



	x = edge[0];
	if (nWindowsVersion <= WINDOWS_XP) {
		x -= 1;
		height -= 2;
	}
	y = rect.bottom - height + 1;
	width = edge[1] - edge[0] - 1;
	if ((fScale > 1.20f) && (fScale <2.40f))
		height -= 1;
	if (nWindowsVersion <= WINDOWS_7)
		height += 1;

	hStatusToolbar = CreateWindowExW(WS_EX_TRANSPARENT, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_TABSTOP | WS_DISABLED |
		TBSTYLE_LIST | CCS_NOPARENTALIGN | CCS_NODIVIDER | CCS_NORESIZE,
		x, y, width, height, hMainDialog, (HMENU)IDC_STATUS_TOOLBAR, hMainInstance, NULL);

	SendMessage(hStatusToolbar, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hStatusToolbar, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_MIXEDBUTTONS);
	SendMessage(hStatusToolbar, TB_SETIMAGELIST, 0, (LPARAM)NULL);
	SendMessage(hStatusToolbar, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)NULL);
	SendMessage(hStatusToolbar, TB_SETBITMAPSIZE, 0, MAKELONG(0,0));

	memset(tbbStatusToolbarButtons, 0, sizeof(TBBUTTON));
	tbbStatusToolbarButtons[0].idCommand = IDC_HASH;
	tbbStatusToolbarButtons[0].fsStyle = BTNS_SHOWTEXT;
	tbbStatusToolbarButtons[0].fsState = TBSTATE_ENABLED;
	tbbStatusToolbarButtons[0].iString = (INT_PTR)L"#";
	SendMessage(hStatusToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hStatusToolbar, TB_ADDBUTTONS, (WPARAM)1, (LPARAM)&tbbStatusToolbarButtons);

	SendMessage(hStatusToolbar, TB_SETBUTTONSIZE, 0, MAKELPARAM(width, height - 1));
	tbi.cbSize = sizeof(tbi);
	tbi.dwMask = TBIF_SIZE | TBIF_COMMAND;
	tbi.cx = (WORD)width;
	tbi.idCommand = IDC_HASH;
	SendMessage(hStatusToolbar, TB_SETBUTTONINFO, (WPARAM)IDC_HASH, (LPARAM)&tbi);

	SetWindowPos(hStatusToolbar, GetDlgItem(hMainDialog, IDCANCEL), x, y, width, height, 0);
	ShowWindow(hStatusToolbar, SW_SHOWNORMAL);
}
