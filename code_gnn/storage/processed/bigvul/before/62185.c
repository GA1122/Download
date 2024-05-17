char* FileDialog(BOOL save, char* path, const ext_t* ext, DWORD options)
{
	DWORD tmp;
	OPENFILENAMEA ofn;
	char selected_name[MAX_PATH];
	char *ext_string = NULL, *all_files = NULL;
	size_t i, j, ext_strlen;
	BOOL r;
	char* filepath = NULL;
	HRESULT hr = FALSE;
	IFileDialog *pfd = NULL;
	IShellItem *psiResult;
	COMDLG_FILTERSPEC* filter_spec = NULL;
	wchar_t *wpath = NULL, *wfilename = NULL;
	IShellItem *si_path = NULL;	 

	if ((ext == NULL) || (ext->count == 0) || (ext->extension == NULL) || (ext->description == NULL))
		return NULL;
	dialog_showing++;

	if (nWindowsVersion >= WINDOWS_VISTA) {
		INIT_VISTA_SHELL32;
		filter_spec = (COMDLG_FILTERSPEC*)calloc(ext->count + 1, sizeof(COMDLG_FILTERSPEC));
		if ((IS_VISTA_SHELL32_AVAILABLE) && (filter_spec != NULL)) {
			for (i = 0; i < ext->count; i++) {
				filter_spec[i].pszSpec = utf8_to_wchar(ext->extension[i]);
				filter_spec[i].pszName = utf8_to_wchar(ext->description[i]);
			}
			filter_spec[i].pszSpec = L"*.*";
			filter_spec[i].pszName = utf8_to_wchar(lmprintf(MSG_107));

			hr = CoCreateInstance(save ? &CLSID_FileSaveDialog : &CLSID_FileOpenDialog, NULL, CLSCTX_INPROC,
				&IID_IFileDialog, (LPVOID)&pfd);

			if (FAILED(hr)) {
				SetLastError(hr);
				uprintf("CoCreateInstance for FileOpenDialog failed: %s\n", WindowsErrorString());
				pfd = NULL;	 
				goto fallback;
			}

			pfd->lpVtbl->SetFileTypes(pfd, (UINT)ext->count + 1, filter_spec);

			wpath = utf8_to_wchar(path);
			hr = (*pfSHCreateItemFromParsingName)(wpath, NULL, &IID_IShellItem, (LPVOID)&si_path);
			if (SUCCEEDED(hr)) {
				pfd->lpVtbl->SetFolder(pfd, si_path);
			}
			safe_free(wpath);

			wfilename = utf8_to_wchar((ext->filename == NULL) ? "" : ext->filename);
			if (wfilename != NULL) {
				pfd->lpVtbl->SetFileName(pfd, wfilename);
			}

			hr = pfd->lpVtbl->Show(pfd, hMainDialog);

			safe_free(wfilename);
			for (i = 0; i < ext->count; i++) {
				safe_free(filter_spec[i].pszSpec);
				safe_free(filter_spec[i].pszName);
			}
			safe_free(filter_spec[i].pszName);
			safe_free(filter_spec);

			if (SUCCEEDED(hr)) {
				hr = pfd->lpVtbl->GetResult(pfd, &psiResult);
				if (SUCCEEDED(hr)) {
					hr = psiResult->lpVtbl->GetDisplayName(psiResult, SIGDN_FILESYSPATH, &wpath);
					if (SUCCEEDED(hr)) {
						filepath = wchar_to_utf8(wpath);
						CoTaskMemFree(wpath);
					} else {
						SetLastError(hr);
						uprintf("Unable to access file path: %s\n", WindowsErrorString());
					}
					psiResult->lpVtbl->Release(psiResult);
				}
			} else if ((hr & 0xFFFF) != ERROR_CANCELLED) {
				SetLastError(hr);
				uprintf("Could not show FileOpenDialog: %s\n", WindowsErrorString());
				goto fallback;
			}
			pfd->lpVtbl->Release(pfd);
			dialog_showing--;
			return filepath;
		}
	fallback:
		safe_free(filter_spec);
		if (pfd != NULL) {
			pfd->lpVtbl->Release(pfd);
		}
	}

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hMainDialog;
	static_sprintf(selected_name, "%s", (ext->filename == NULL)?"":ext->filename);
	ofn.lpstrFile = selected_name;
	ofn.nMaxFile = MAX_PATH;
	all_files = lmprintf(MSG_107);
	ext_strlen = 0;
	for (i=0; i<ext->count; i++) {
		ext_strlen += safe_strlen(ext->description[i]) + 2*safe_strlen(ext->extension[i]) + sizeof(" ()\r\r");
	}
	ext_strlen += safe_strlen(all_files) + sizeof(" (*.*)\r*.*\r");
	ext_string = (char*)malloc(ext_strlen+1);
	if (ext_string == NULL)
		return NULL;
	ext_string[0] = 0;
	for (i=0, j=0; i<ext->count; i++) {
		j += _snprintf(&ext_string[j], ext_strlen-j, "%s (%s)\r%s\r", ext->description[i], ext->extension[i], ext->extension[i]);
	}
	j = _snprintf(&ext_string[j], ext_strlen-j, "%s (*.*)\r*.*\r", all_files);
	for (i=0; i<ext_strlen; i++) {
#if defined(_MSC_VER)
#pragma warning(suppress: 6385)
#endif
		if (ext_string[i] == '\r') {
#if defined(_MSC_VER)
#pragma warning(suppress: 6386)
#endif
			ext_string[i] = 0;
		}
	}
	ofn.lpstrFilter = ext_string;
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = path;
	ofn.Flags = OFN_OVERWRITEPROMPT | options;
	if (save) {
		r = GetSaveFileNameU(&ofn);
	} else {
		r = GetOpenFileNameU(&ofn);
	}
	if (r) {
		filepath = safe_strdup(selected_name);
	} else {
		tmp = CommDlgExtendedError();
		if (tmp != 0) {
			uprintf("Could not select file for %s. Error %X\n", save?"save":"open", tmp);
		}
	}
	safe_free(ext_string);
	dialog_showing--;
	return filepath;
}