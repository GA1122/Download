void AddClearBrowsingDataStrings(content::WebUIDataSource* html_source,
                                 Profile* profile) {
  int clear_cookies_summary_msg_id =
      IDS_SETTINGS_CLEAR_COOKIES_AND_SITE_DATA_SUMMARY_BASIC;

#if defined(OS_CHROMEOS)
  if (AccountConsistencyModeManager::IsMirrorEnabledForProfile(profile)) {
    clear_cookies_summary_msg_id =
        IDS_SETTINGS_CLEAR_COOKIES_AND_SITE_DATA_SUMMARY_BASIC_WITH_EXCEPTION;
  }
#else   
  if (signin::IsDiceEnabledForProfile(profile->GetPrefs())) {
    clear_cookies_summary_msg_id =
        IDS_SETTINGS_CLEAR_COOKIES_AND_SITE_DATA_SUMMARY_BASIC_WITH_EXCEPTION;
  }
#endif

  LocalizedString localized_strings[] = {
      {"clearTimeRange", IDS_SETTINGS_CLEAR_PERIOD_TITLE},
      {"clearBrowsingHistory", IDS_SETTINGS_CLEAR_BROWSING_HISTORY},
      {"clearBrowsingHistorySummary",
       IDS_SETTINGS_CLEAR_BROWSING_HISTORY_SUMMARY},
      {"clearDownloadHistory", IDS_SETTINGS_CLEAR_DOWNLOAD_HISTORY},
      {"clearCache", IDS_SETTINGS_CLEAR_CACHE},
      {"clearCookies", IDS_SETTINGS_CLEAR_COOKIES},
      {"clearCookiesSummary", clear_cookies_summary_msg_id},
      {"clearCookiesCounter", IDS_DEL_COOKIES_COUNTER},
      {"clearCookiesFlash", IDS_SETTINGS_CLEAR_COOKIES_FLASH},
      {"clearPasswords", IDS_SETTINGS_CLEAR_PASSWORDS},
      {"clearFormData", IDS_SETTINGS_CLEAR_FORM_DATA},
      {"clearHostedAppData", IDS_SETTINGS_CLEAR_HOSTED_APP_DATA},
      {"clearMediaLicenses", IDS_SETTINGS_CLEAR_MEDIA_LICENSES},
      {"clearPeriodHour", IDS_SETTINGS_CLEAR_PERIOD_HOUR},
      {"clearPeriod24Hours", IDS_SETTINGS_CLEAR_PERIOD_24_HOURS},
      {"clearPeriod7Days", IDS_SETTINGS_CLEAR_PERIOD_7_DAYS},
      {"clearPeriod4Weeks", IDS_SETTINGS_CLEAR_PERIOD_FOUR_WEEKS},
      {"clearPeriodEverything", IDS_SETTINGS_CLEAR_PERIOD_EVERYTHING},
      {"historyDeletionDialogTitle",
       IDS_CLEAR_BROWSING_DATA_HISTORY_NOTICE_TITLE},
      {"historyDeletionDialogOK", IDS_CLEAR_BROWSING_DATA_HISTORY_NOTICE_OK},
      {"importantSitesSubtitleCookies",
       IDS_SETTINGS_IMPORTANT_SITES_SUBTITLE_COOKIES},
      {"importantSitesSubtitleCookiesAndCache",
       IDS_SETTINGS_IMPORTANT_SITES_SUBTITLE_COOKIES_AND_CACHE},
      {"importantSitesConfirm", IDS_SETTINGS_IMPORTANT_SITES_CONFIRM},
      {"notificationWarning", IDS_SETTINGS_NOTIFICATION_WARNING},
  };

  html_source->AddString(
      "clearBrowsingHistorySummarySignedIn",
      l10n_util::GetStringFUTF16(
          IDS_SETTINGS_CLEAR_BROWSING_HISTORY_SUMMARY_SIGNED_IN,
          base::ASCIIToUTF16(chrome::kMyActivityUrlInClearBrowsingData)));
  html_source->AddString(
      "clearBrowsingHistorySummarySynced",
      l10n_util::GetStringFUTF16(
          IDS_SETTINGS_CLEAR_BROWSING_HISTORY_SUMMARY_SYNCED,
          base::ASCIIToUTF16(chrome::kMyActivityUrlInClearBrowsingData)));
  html_source->AddString(
      "historyDeletionDialogBody",
      l10n_util::GetStringFUTF16(
          IDS_CLEAR_BROWSING_DATA_HISTORY_NOTICE,
          l10n_util::GetStringUTF16(
              IDS_SETTINGS_CLEAR_DATA_MYACTIVITY_URL_IN_DIALOG)));

  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));
}
