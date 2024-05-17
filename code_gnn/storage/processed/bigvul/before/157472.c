base::string16 GetChromiumModelIdForProfile(
    const base::FilePath& profile_path) {
  return GetAppModelIdForProfile(
      ShellUtil::GetBrowserModelId(InstallUtil::IsPerUserInstall()),
      profile_path);
}
