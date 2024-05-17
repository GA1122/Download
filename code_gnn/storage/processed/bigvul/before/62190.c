LPCDLGTEMPLATE GetDialogTemplate(int Dialog_ID)
{
	int i;
	const char thai_id[] = "th-TH";
	size_t len;
	DWORD size;
	DWORD* dwBuf;
	WCHAR* wBuf;
	LPCDLGTEMPLATE rcTemplate = (LPCDLGTEMPLATE) GetResource(hMainInstance, MAKEINTRESOURCEA(Dialog_ID),
		_RT_DIALOG, get_name_from_id(Dialog_ID), &size, TRUE);

	if ((size == 0) || (rcTemplate == NULL)) {
		safe_free(rcTemplate);
		return NULL;
	}
	if (right_to_left_mode) {
		dwBuf = (DWORD*)rcTemplate;
		dwBuf[2] = WS_EX_RTLREADING | WS_EX_APPWINDOW | WS_EX_LAYOUTRTL;
	}


	if (IsFontAvailable("Segoe UI Symbol") && (selected_locale != NULL)
		&& (safe_strcmp(selected_locale->txt[0], thai_id) == 0))
		return rcTemplate;

	wBuf = (WCHAR*)rcTemplate;
	wBuf = &wBuf[14];	 
	for (i = 0; i<2; i++) {
		if (*wBuf == 0xFFFF)
			wBuf = &wBuf[2];	 
		else
			wBuf = &wBuf[wcslen(wBuf) + 1];  
	}
	wBuf = &wBuf[3];
	if (wcscmp(L"Segoe UI Symbol", wBuf) == 0) {
		uintptr_t src, dst, start = (uintptr_t)rcTemplate;
		if ((nWindowsVersion > WINDOWS_XP) && IsFontAvailable("Segoe UI")) {
			wBuf[8] = 0;
		} else {
			wcscpy(wBuf, L"MS Shell Dlg");
		}
		len = wcslen(wBuf);
		wBuf[len + 1] = 0;
		dst = (uintptr_t)&wBuf[len + 2];
		dst &= ~3;
		src = (uintptr_t)&wBuf[17];
		src &= ~3;
		memmove((void*)dst, (void*)src, size - (src - start));
	} else {
		uprintf("Could not locate font for %s!", get_name_from_id(Dialog_ID));
	}
	return rcTemplate;
}
