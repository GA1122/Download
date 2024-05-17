CWD_API int php_sys_readlink(const char *link, char *target, size_t target_len){  
	HINSTANCE kernel32;
	HANDLE hFile;
	DWORD dwRet;

	typedef BOOL (WINAPI *gfpnh_func)(HANDLE, LPTSTR, DWORD, DWORD);
	gfpnh_func pGetFinalPathNameByHandle;

	kernel32 = LoadLibrary("kernel32.dll");

	if (kernel32) {
		pGetFinalPathNameByHandle = (gfpnh_func)GetProcAddress(kernel32, "GetFinalPathNameByHandleA");
		if (pGetFinalPathNameByHandle == NULL) {
			return -1;
		}
	} else {
		return -1;
	}

	hFile = CreateFile(link,             
				 GENERIC_READ,           
				 FILE_SHARE_READ,        
				 NULL,                   
				 OPEN_EXISTING,          
				 FILE_FLAG_BACKUP_SEMANTICS,  
				 NULL);                  

	if( hFile == INVALID_HANDLE_VALUE) {
			return -1;
	}

	dwRet = pGetFinalPathNameByHandle(hFile, target, MAXPATHLEN, VOLUME_NAME_DOS);
	if(dwRet >= MAXPATHLEN || dwRet == 0) {
		return -1;
	}

	CloseHandle(hFile);

	if(dwRet > 4) {
		 
		if(target[0] == '\\' && target[1] == '\\' && target[2] == '?' && target[3] ==  '\\') {
			char tmp[MAXPATHLEN];
			unsigned int offset = 4;
			dwRet -= 4;

			 
			if (dwRet > 7 && target[4] == 'U' && target[5] == 'N' && target[6] == 'C') {
				offset += 2;
				dwRet -= 2;
				target[offset] = '\\';
			}

			memcpy(tmp, target + offset, dwRet);
			memcpy(target, tmp, dwRet);
		}
	}

	target[dwRet] = '\0';
	return dwRet;
}
 
