base::FilePath GetChromePathForInstallationLevel(InstallationLevel level) {
  return FindExeRelativeToSetupExe(
      GetSetupExeForInstallationLevel(level), kChromeExe);
}
