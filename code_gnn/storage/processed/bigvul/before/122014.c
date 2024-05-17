base::FilePath GetSetupExeFromRegistry(InstallationLevel level,
                                       const wchar_t* app_guid) {
  string16 uninstall;
  if (GetClientStateValue(level, app_guid, kUninstallStringField, &uninstall)) {
    base::FilePath setup_exe_path(uninstall);
    if (file_util::PathExists(setup_exe_path))
      return setup_exe_path;
  }
  return base::FilePath();
}
