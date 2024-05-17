ExtensionUninstaller::ExtensionUninstaller(
    Profile* profile,
    const std::string& extension_id,
    AppListControllerDelegate* controller)
    : profile_(profile),
      app_id_(extension_id),
      controller_(controller) {
}
