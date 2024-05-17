void AddChromeCleanupStrings(content::WebUIDataSource* html_source) {
  const wchar_t kUnwantedSoftwareProtectionWhitePaperUrl[] =
      L"https://www.google.ca/chrome/browser/privacy/"
      "whitepaper.html#unwantedsoftware";

  LocalizedString localized_strings[] = {
      {"chromeCleanupPageTitle",
       IDS_SETTINGS_RESET_CLEAN_UP_COMPUTER_PAGE_TITLE},
      {"chromeCleanupDetailsFilesAndPrograms",
       IDS_SETTINGS_RESET_CLEANUP_DETAILS_FILES_AND_PROGRAMS},
      {"chromeCleanupDetailsRegistryEntries",
       IDS_SETTINGS_RESET_CLEANUP_DETAILS_REGISTRY_ENTRIES},
      {"chromeCleanupExplanationCleanupError",
       IDS_SETTINGS_RESET_CLEANUP_EXPLANATION_CLEANUP_ERROR},
      {"chromeCleanupExplanationFindAndRemove",
       IDS_SETTINGS_RESET_CLEANUP_EXPLANATION_FIND_AND_REMOVE},
      {"chromeCleanupExplanationNoInternet",
       IDS_SETTINGS_RESET_CLEANUP_EXPLANATION_NO_INTERNET_CONNECTION},
      {"chromeCleanupExplanationPermissionsNeeded",
       IDS_SETTINGS_RESET_CLEANUP_EXPLANATION_PERMISSIONS_NEEDED},
      {"chromeCleanupExplanationRemove",
       IDS_CHROME_CLEANUP_PROMPT_EXPLANATION},
      {"chromeCleanupExplanationRemoving",
       IDS_SETTINGS_RESET_CLEANUP_EXPLANATION_CURRENTLY_REMOVING},
      {"chromeCleanupExplanationScanError",
       IDS_SETTINGS_RESET_CLEANUP_EXPLANATION_SCAN_ERROR},
      {"chromeCleanupFindButtonLable",
       IDS_SETTINGS_RESET_CLEANUP_FIND_BUTTON_LABEL},
      {"chromeCleanupLinkShowItems",
       IDS_SETTINGS_RESET_CLEANUP_LINK_SHOW_FILES},
      {"chromeCleanupLogsUploadPermission", IDS_CHROME_CLEANUP_LOGS_PERMISSION},
      {"chromeCleanupRemoveButtonLabel",
       IDS_SETTINGS_RESET_CLEANUP_REMOVE_BUTTON_LABEL},
      {"chromeCleanupRestartButtonLabel",
       IDS_SETTINGS_RESET_CLEANUP_RESTART_BUTTON_LABEL},
      {"chromeCleanupTitleErrorCantRemove",
       IDS_SETTINGS_RESET_CLEANUP_TITLE_ERROR_CANT_REMOVE},
      {"chromeCleanupTitleErrorPermissions",
       IDS_SETTINGS_RESET_CLEANUP_TITLE_ERROR_PERMISSIONS_NEEDED},
      {"chromeCleanupTitleFindAndRemove",
       IDS_SETTINGS_RESET_CLEANUP_TITLE_FIND_AND_REMOVE},
      {"chromeCleanupTitleNoInternet",
       IDS_SETTINGS_RESET_CLEANUP_TITLE_NO_INTERNET_CONNECTION},
      {"chromeCleanupTitleNothingFound",
       IDS_SETTINGS_RESET_CLEANUP_TITLE_NOTHING_FOUND},
      {"chromeCleanupTitleRemove", IDS_SETTINGS_RESET_CLEANUP_TITLE_REMOVE},
      {"chromeCleanupTitleRemoved",
       safe_browsing::UserInitiatedCleanupsEnabled()
           ? IDS_SETTINGS_RESET_CLEANUP_TITLE_DONE
           : IDS_SETTINGS_RESET_CLEANUP_TITLE_REMOVED},
      {"chromeCleanupTitleRemoving", IDS_SETTINGS_RESET_CLEANUP_TITLE_REMOVING},
      {"chromeCleanupTitleRestart", IDS_SETTINGS_RESET_CLEANUP_TITLE_RESTART},
      {"chromeCleanupTitleScanning", IDS_SETTINGS_RESET_CLEANUP_TITLE_SCANNING},
      {"chromeCleanupTitleScanningFailed",
       IDS_SETTINGS_RESET_CLEANUP_TITLE_ERROR_SCANNING_FAILED},
      {"chromeCleanupTitleTryAgainButtonLabel",
       IDS_SETTINGS_RESET_CLEANUP_TRY_AGAIN_BUTTON_LABEL},
      {"chromeCleanupTitleLogsPermissionExplanation",
       IDS_SETTINGS_RESET_CLEANUP_LOGS_PERMISSION_EXPLANATION},
      {"chromeCleanupTitleCleanupUnavailable",
       IDS_SETTINGS_RESET_CLEANUP_TITLE_CLEANUP_UNAVAILABLE},
      {"chromeCleanupExplanationCleanupUnavailable",
       IDS_SETTINGS_RESET_CLEANUP_EXPLANATION_CLEANUP_UNAVAILABLE},
  };

  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));
  const std::string cleanup_learn_more_url =
      google_util::AppendGoogleLocaleParam(
          GURL(chrome::kChromeCleanerLearnMoreURL),
          g_browser_process->GetApplicationLocale())
          .spec();
  html_source->AddString("chromeCleanupLearnMoreUrl", cleanup_learn_more_url);

  const base::string16 powered_by_html =
      l10n_util::GetStringFUTF16(IDS_SETTINGS_RESET_CLEANUP_FOOTER_POWERED_BY,
                                 L"<span id='powered-by-logo'></span>");
  html_source->AddString("chromeCleanupPoweredByHtml", powered_by_html);

  const base::string16 cleanup_details_explanation =
      l10n_util::GetStringFUTF16(IDS_SETTINGS_RESET_CLEANUP_DETAILS_EXPLANATION,
                                 kUnwantedSoftwareProtectionWhitePaperUrl);
  html_source->AddString("chromeCleanupDetailsExplanation",
                         cleanup_details_explanation);
}
