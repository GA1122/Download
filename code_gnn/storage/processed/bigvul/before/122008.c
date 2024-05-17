base::FilePath GetAppHostPathForInstallationLevel(InstallationLevel level) {
  return FindExeRelativeToSetupExe(
      GetSetupExeFromRegistry(level, kAppHostAppId), kChromeAppHostExe);
}
