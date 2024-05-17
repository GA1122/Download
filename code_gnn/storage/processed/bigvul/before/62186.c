void FlashTaskbar(HANDLE handle)
{
	FLASHWINFO pf;

	if (handle == NULL)
		return;
	pf.cbSize = sizeof(FLASHWINFO);
	pf.hwnd = handle;
	pf.dwFlags = FLASHW_TIMER | FLASHW_TRAY;
	pf.uCount = 5;
	pf.dwTimeout = 75;
	FlashWindowEx(&pf);
}
