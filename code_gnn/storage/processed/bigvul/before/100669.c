void CheckUserDataDirPolicy(FilePath* user_data_dir) {
  DCHECK(user_data_dir);
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  const bool is_chrome_frame =
      !user_data_dir->empty() &&
      command_line->HasSwitch(switches::kChromeFrame) &&
      command_line->HasSwitch(switches::kAutomationClientChannelID);

  FilePath cf_host_dir;
  if (is_chrome_frame)
    cf_host_dir = user_data_dir->BaseName();

  const char* key_name_ascii = (is_chrome_frame ? policy::key::kGCFUserDataDir :
                                policy::key::kUserDataDir);
  std::wstring key_name(ASCIIToWide(key_name_ascii));
  if (LoadUserDataDirPolicyFromRegistry(HKEY_LOCAL_MACHINE, key_name,
                                        user_data_dir) ||
      LoadUserDataDirPolicyFromRegistry(HKEY_CURRENT_USER, key_name,
                                        user_data_dir)) {
    if (is_chrome_frame)
      *user_data_dir = user_data_dir->Append(cf_host_dir);
  }
}
