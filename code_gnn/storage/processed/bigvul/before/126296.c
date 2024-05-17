void BrowserCommandController::UpdateCommandsForIncognitoAvailability() {
  IncognitoModePrefs::Availability incognito_availability =
      IncognitoModePrefs::GetAvailability(profile()->GetPrefs());
  command_updater_.UpdateCommandEnabled(
      IDC_NEW_WINDOW,
      incognito_availability != IncognitoModePrefs::FORCED);
  command_updater_.UpdateCommandEnabled(
      IDC_NEW_INCOGNITO_WINDOW,
      incognito_availability != IncognitoModePrefs::DISABLED);

  const bool command_enabled =
      incognito_availability != IncognitoModePrefs::FORCED;
  command_updater_.UpdateCommandEnabled(
      IDC_SHOW_BOOKMARK_MANAGER,
      browser_defaults::bookmarks_enabled && command_enabled);
  ExtensionService* extension_service = profile()->GetExtensionService();
  bool enable_extensions =
      extension_service && extension_service->extensions_enabled();
  command_updater_.UpdateCommandEnabled(IDC_MANAGE_EXTENSIONS,
                                        enable_extensions && command_enabled);

  const bool show_main_ui =
      IsShowingMainUI(window() && window()->IsFullscreen());
  command_updater_.UpdateCommandEnabled(IDC_IMPORT_SETTINGS,
                                        show_main_ui && command_enabled);
  command_updater_.UpdateCommandEnabled(IDC_OPTIONS,
                                        show_main_ui && command_enabled);
}
