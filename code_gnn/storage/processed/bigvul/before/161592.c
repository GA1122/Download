void AddSystemStrings(content::WebUIDataSource* html_source) {
  LocalizedString localized_strings[] = {
    {"systemPageTitle", IDS_SETTINGS_SYSTEM},
#if !defined(OS_MACOSX)
    {"backgroundAppsLabel", IDS_SETTINGS_SYSTEM_BACKGROUND_APPS_LABEL},
#endif
    {"hardwareAccelerationLabel",
     IDS_SETTINGS_SYSTEM_HARDWARE_ACCELERATION_LABEL},
    {"proxySettingsLabel", IDS_SETTINGS_SYSTEM_PROXY_SETTINGS_LABEL},
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

  SystemHandler::AddLoadTimeData(html_source);
}
