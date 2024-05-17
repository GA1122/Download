void AddDownloadsStrings(content::WebUIDataSource* html_source) {
  LocalizedString localized_strings[] = {
      {"downloadsPageTitle", IDS_SETTINGS_DOWNLOADS},
      {"downloadLocation", IDS_SETTINGS_DOWNLOAD_LOCATION},
      {"changeDownloadLocation", IDS_SETTINGS_CHANGE_DOWNLOAD_LOCATION},
      {"promptForDownload", IDS_SETTINGS_PROMPT_FOR_DOWNLOAD},
      {"disconnectGoogleDriveAccount", IDS_SETTINGS_DISCONNECT_GOOGLE_DRIVE},
      {"openFileTypesAutomatically",
       IDS_SETTINGS_OPEN_FILE_TYPES_AUTOMATICALLY}};
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));
}
