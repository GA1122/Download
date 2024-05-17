INT_PTR MyDialogBox(HINSTANCE hInstance, int Dialog_ID, HWND hWndParent, DLGPROC lpDialogFunc)
{
	INT_PTR ret;
	LPCDLGTEMPLATE rcTemplate = GetDialogTemplate(Dialog_ID);

	ShowWindow(hMainDialog, SW_NORMAL);

	ret = DialogBoxIndirect(hMainInstance, rcTemplate, hWndParent, lpDialogFunc);
	safe_free(rcTemplate);
	return ret;
}
