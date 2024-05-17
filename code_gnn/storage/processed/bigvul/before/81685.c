get_system_name(char **sysName)
{
#if defined(_WIN32)
#if !defined(__SYMBIAN32__)
#if defined(_WIN32_WCE)
	*sysName = mg_strdup("WinCE");
#else
	char name[128];
	DWORD dwVersion = 0;
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	DWORD dwBuild = 0;
	BOOL wowRet, isWoW = FALSE;

#if defined(_MSC_VER)
#pragma warning(push)
 
#pragma warning(disable : 4996)
#endif
	dwVersion = GetVersion();
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	dwBuild = ((dwVersion < 0x80000000) ? (DWORD)(HIWORD(dwVersion)) : 0);
	(void)dwBuild;

	wowRet = IsWow64Process(GetCurrentProcess(), &isWoW);

	sprintf(name,
	        "Windows %u.%u%s",
	        (unsigned)dwMajorVersion,
	        (unsigned)dwMinorVersion,
	        (wowRet ? (isWoW ? " (WoW64)" : "") : " (?)"));

	*sysName = mg_strdup(name);
#endif
#else
	*sysName = mg_strdup("Symbian");
#endif
#else
	struct utsname name;
	memset(&name, 0, sizeof(name));
	uname(&name);
	*sysName = mg_strdup(name.sysname);
#endif
}