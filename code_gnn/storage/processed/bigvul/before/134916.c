void RegisterStubPathOverridesIfNecessary() {
  DCHECK(!g_browser_process);

  base::FilePath user_data_dir;
  if (base::SysInfo::IsRunningOnChromeOS() ||
      !base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir)) {
    return;
  }

  chromeos::RegisterStubPathOverrides(user_data_dir);
}
