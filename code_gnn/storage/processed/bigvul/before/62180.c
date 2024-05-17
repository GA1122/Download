BOOL CreateTooltip(HWND hControl, const char* message, int duration)
{
	TOOLINFOW toolInfo = {0};
	int i;

	if ( (hControl == NULL) || (message == NULL) ) {
		return FALSE;
	}

	DestroyTooltip(hControl);

	for (i=0; i<MAX_TOOLTIPS; i++) {
		if (ttlist[i].hTip == NULL) break;
	}
	if (i >= MAX_TOOLTIPS) {
		uprintf("Maximum number of tooltips reached (%d)\n", MAX_TOOLTIPS);
		return FALSE;
	}

	ttlist[i].hTip = CreateWindowExW(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMainDialog, NULL,
		hMainInstance, NULL);

	if (ttlist[i].hTip == NULL) {
		return FALSE;
	}
	ttlist[i].hCtrl = hControl;

	ttlist[i].original_proc = (WNDPROC)SetWindowLongPtr(ttlist[i].hTip, GWLP_WNDPROC, (LONG_PTR)TooltipCallback);

	ttlist[i].wstring = utf8_to_wchar(message);

	PostMessage(ttlist[i].hTip, TTM_SETDELAYTIME, (WPARAM)TTDT_AUTOPOP, (LPARAM)duration);

	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = ttlist[i].hTip;	 
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS | ((right_to_left_mode)?TTF_RTLREADING:0);
	if (!(SendMessage(hControl, WM_GETDLGCODE, 0, 0) & DLGC_BUTTON))
		toolInfo.uFlags |= 0x80000000L | TTF_CENTERTIP;

	toolInfo.uId = (UINT_PTR)hControl;
	toolInfo.lpszText = LPSTR_TEXTCALLBACKW;
	SendMessageW(ttlist[i].hTip, TTM_ADDTOOLW, 0, (LPARAM)&toolInfo);

	return TRUE;
}
