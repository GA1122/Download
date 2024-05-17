base::FilePath GetAnyAppHostPath() {
  base::FilePath app_host_path;
#ifndef OFFICIAL_BUILD
  app_host_path = GetDevelopmentExe(kChromeAppHostExe);
#endif
  if (app_host_path.empty()) {
    app_host_path = GetAppHostPathForInstallationLevel(
        SYSTEM_LEVEL_INSTALLATION);
  }
  if (app_host_path.empty())
    app_host_path = GetAppHostPathForInstallationLevel(USER_LEVEL_INSTALLATION);
  return app_host_path;
}
