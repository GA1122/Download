void AppListControllerDelegate::UninstallApp(Profile* profile,
                                             const std::string& app_id) {
  ExtensionUninstaller* uninstaller =
      new ExtensionUninstaller(profile, app_id, this);
  uninstaller->Run();
}
