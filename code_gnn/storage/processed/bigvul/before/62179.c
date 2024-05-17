BOOL CreateTaskbarList(void)
{
	HRESULT hr;
	if (nWindowsVersion < WINDOWS_7)
		return FALSE;
	hr = CoCreateInstance(&my_CLSID_TaskbarList, NULL, CLSCTX_ALL, &my_IID_ITaskbarList3, (LPVOID)&ptbl);
	if (FAILED(hr)) {
		uprintf("CoCreateInstance for TaskbarList failed: error %X\n", hr);
		ptbl = NULL;
		return FALSE;
	}
	return TRUE;
}