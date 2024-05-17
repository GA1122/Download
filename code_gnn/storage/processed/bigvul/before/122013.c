 base::FilePath GetSetupExeForInstallationLevel(InstallationLevel level) {
   base::FilePath setup_exe_path(
      GetSetupExeFromRegistry(level, kBinariesAppGuid));
  if (setup_exe_path.empty())
    setup_exe_path = GetSetupExeFromRegistry(level, kBrowserAppGuid);
  return setup_exe_path;
}
