int SelectionDyn(char* title, char* message, char** szChoice, int nChoices)
{
#define ID_RADIO  12345
	LPCWSTR lpwszTypeFace = L"MS Shell Dlg";
	LPDLGTEMPLATEA lpdt;
	LPDLGITEMTEMPLATEA lpdit;
	LPCWSTR lpwszCaption;
	LPWORD lpw;
	LPWSTR lpwsz;
	int i, ret, nchar;

	lpdt = (LPDLGTEMPLATE)calloc(512 + nChoices * 256, 1);

	lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION | DS_MODALFRAME | DS_CENTER | DS_SHELLFONT;
	lpdt->cdit = 2 + nChoices;
	lpdt->x = 10;
	lpdt->y = 10;
	lpdt->cx = 300;
	lpdt->cy = 100;

	lpw = (LPWORD)(&lpdt[1]);
	*lpw++ = 0;		 
	*lpw++ = 0;		 
	lpwsz = (LPWSTR)lpw;
	nchar = MultiByteToWideChar(CP_UTF8, 0, title, -1, lpwsz, 50);
	lpw += nchar;

	if (lpdt->style & (DS_SETFONT | DS_SHELLFONT)) {
		*lpw++ = 8;
		for (lpwsz = (LPWSTR)lpw, lpwszCaption = lpwszTypeFace; (*lpwsz++ = *lpwszCaption++) != 0; );
		lpw = (LPWORD)lpwsz;
	}

	lpw = lpwAlign(lpw);
	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
	lpdit->x = 10;
	lpdit->y = 70;
	lpdit->cx = 50;
	lpdit->cy = 14;
	lpdit->id = IDOK;

	lpw = (LPWORD)(&lpdit[1]);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;	 

	lpwsz = (LPWSTR)lpw;
	nchar = MultiByteToWideChar(CP_UTF8, 0, "OK", -1, lpwsz, 50);
	lpw += nchar;
	*lpw++ = 0;		 

	lpw = lpwAlign(lpw);
	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->x = 90;
	lpdit->y = 70;
	lpdit->cx = 50;
	lpdit->cy = 14;
	lpdit->id = IDCANCEL;
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	lpw = (LPWORD)(&lpdit[1]);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;

	lpwsz = (LPWSTR)lpw;
	nchar = MultiByteToWideChar(CP_UTF8, 0, lmprintf(MSG_007), -1, lpwsz, 50);
	lpw += nchar;
	*lpw++ = 0;

	for (i = 0; i < nChoices; i++) {
		lpw = lpwAlign(lpw);
		lpdit = (LPDLGITEMTEMPLATE)lpw;
		lpdit->x = 10;
		lpdit->y = 10 + 15 * i;
		lpdit->cx = 40;
		lpdit->cy = 20;
		lpdit->id = ID_RADIO;
		lpdit->style = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | (i == 0 ? WS_GROUP : 0);

		lpw = (LPWORD)(&lpdit[1]);
		*lpw++ = 0xFFFF;
		*lpw++ = 0x0080;

		lpwsz = (LPWSTR)lpw;
		nchar = MultiByteToWideChar(CP_UTF8, 0, szChoice[i], -1, lpwsz, 150);
		lpw += nchar;
		*lpw++ = 0;
	}

	ret = (int)DialogBoxIndirect(hMainInstance, (LPDLGTEMPLATE)lpdt, hMainDialog, (DLGPROC)SelectionDynCallback);
	free(lpdt);
	return ret;
}
