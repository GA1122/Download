BOOL SetFormatPromptHook(void)
{
	HMODULE mui_lib;
	char mui_path[MAX_PATH];
	static char title_str[128], button_str[128];

	if (fp_weh != NULL)
		return TRUE;	 

	static_sprintf(mui_path, "%s\\%s\\shell32.dll.mui", system_dir, GetCurrentMUI());
	mui_lib = LoadLibraryU(mui_path);
	if (mui_lib != NULL) {
		if (LoadStringU(mui_lib, 4125, title_str, sizeof(title_str)) > 0)
			fp_title_str = title_str;
		else
			uprintf("Warning: Could not locate localized format prompt title string in '%s': %s", mui_path, WindowsErrorString());
		if (LoadStringU(mui_lib, 4126, button_str, sizeof(button_str)) > 0)
			fp_button_str = button_str;
		else
			uprintf("Warning: Could not locate localized format prompt button string in '%s': %s", mui_path, WindowsErrorString());
		FreeLibrary(mui_lib);
	}

	fp_weh = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL,
		FormatPromptHook, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	return (fp_weh != NULL);
}
