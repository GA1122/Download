SIZE GetTextSize(HWND hCtrl)
{
	SIZE sz = {0, 0};
	HDC hDC;
	wchar_t *wstr = NULL;
	int len;
	HFONT hFont;

	hDC = GetDC(hCtrl);
	if (hDC == NULL)
		goto out;
	hFont = (HFONT)SendMessageA(hCtrl, WM_GETFONT, 0, 0);
	if (hFont == NULL)
		goto out;
	SelectObject(hDC, hFont);
	len = GetWindowTextLengthW(hCtrl);
	if (len <= 0)
		goto out;
	wstr = calloc(len + 1, sizeof(wchar_t));
	if (wstr == NULL)
		goto out;
	if (GetWindowTextW(hCtrl, wstr, len + 1) > 0)
		GetTextExtentPoint32W(hDC, wstr, len, &sz);
out:
	safe_free(wstr);
	if (hDC != NULL)
		ReleaseDC(hCtrl, hDC);
	return sz;
}