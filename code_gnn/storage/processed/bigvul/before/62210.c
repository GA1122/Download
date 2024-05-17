BOOL SetUpdateCheck(void)
{
	BOOL enable_updates;
	uint64_t commcheck = _GetTickCount64();
	notification_info more_info = { IDD_UPDATE_POLICY, UpdateCallback };
	char filename[MAX_PATH] = "", exename[] = APPLICATION_NAME ".exe";
	size_t fn_len, exe_len;

	WriteSetting64(SETTING_COMM_CHECK, commcheck);
	if (ReadSetting64(SETTING_COMM_CHECK) != commcheck)
		return FALSE;
	settings_commcheck = TRUE;

	if (ReadSetting32(SETTING_UPDATE_INTERVAL) == 0) {

		GetModuleFileNameU(NULL, filename, sizeof(filename));
		fn_len = safe_strlen(filename);
		exe_len = safe_strlen(exename);
#if !defined(_DEBUG)	 
		if ((fn_len > exe_len) && (safe_stricmp(&filename[fn_len-exe_len], exename) == 0)) {
			uprintf("Short name used - Disabling initial update policy prompt\n");
			enable_updates = TRUE;
		} else {
#endif
			enable_updates = Notification(MSG_QUESTION, &more_info, lmprintf(MSG_004), lmprintf(MSG_005));
#if !defined(_DEBUG)
		}
#endif
		if (!enable_updates) {
			WriteSetting32(SETTING_UPDATE_INTERVAL, -1);
			return FALSE;
		}
		if ( (ReadSetting32(SETTING_UPDATE_INTERVAL) == 0) ||
			 ((ReadSetting32(SETTING_UPDATE_INTERVAL) == -1) && enable_updates) )
			WriteSetting32(SETTING_UPDATE_INTERVAL, 86400);
	}
	return TRUE;
}
