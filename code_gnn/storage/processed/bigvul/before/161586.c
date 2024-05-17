void AddPrivacyStrings(content::WebUIDataSource* html_source,
                       Profile* profile) {
  LocalizedString localized_strings[] = {
    {"privacyPageTitle", IDS_SETTINGS_PRIVACY},
    {"linkDoctorPref", IDS_SETTINGS_LINKDOCTOR_PREF},
    {"searchSuggestPref", IDS_SETTINGS_SUGGEST_PREF},
    {"networkPredictionEnabled",
     IDS_SETTINGS_NETWORK_PREDICTION_ENABLED_DESCRIPTION},
    {"safeBrowsingEnableProtection",
     IDS_SETTINGS_SAFEBROWSING_ENABLEPROTECTION},
    {"spellingPref", IDS_SETTINGS_SPELLING_PREF},
    {"spellingDescription", IDS_SETTINGS_SPELLING_DESCRIPTION},
#if defined(OS_CHROMEOS)
    {"enableLogging", IDS_SETTINGS_ENABLE_LOGGING_DIAGNOSTIC_AND_USAGE_DATA},
#else
    {"enableLogging", IDS_SETTINGS_ENABLE_LOGGING},
#endif
    {"doNotTrack", IDS_SETTINGS_ENABLE_DO_NOT_TRACK},
    {"doNotTrackDialogTitle", IDS_SETTINGS_ENABLE_DO_NOT_TRACK_DIALOG_TITLE},
    {"enableContentProtectionAttestation",
     IDS_SETTINGS_ENABLE_CONTENT_PROTECTION_ATTESTATION},
    {"wakeOnWifi", IDS_SETTINGS_WAKE_ON_WIFI_DESCRIPTION},
    {"manageCertificates", IDS_SETTINGS_MANAGE_CERTIFICATES},
    {"manageCertificatesDescription",
     IDS_SETTINGS_MANAGE_CERTIFICATES_DESCRIPTION},
    {"contentSettings", IDS_SETTINGS_CONTENT_SETTINGS},
    {"siteSettings", IDS_SETTINGS_SITE_SETTINGS},
    {"siteSettingsDescription", IDS_SETTINGS_SITE_SETTINGS_DESCRIPTION},
    {"clearData", IDS_SETTINGS_CLEAR_DATA},
    {"clearBrowsingData", IDS_SETTINGS_CLEAR_BROWSING_DATA},
    {"clearBrowsingDataDescription", IDS_SETTINGS_CLEAR_DATA_DESCRIPTION},
    {"titleAndCount", IDS_SETTINGS_TITLE_AND_COUNT},
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

  html_source->AddBoolean(
      "importantSitesInCbd",
      base::FeatureList::IsEnabled(features::kImportantSitesInCbd));

  html_source->AddLocalizedString(
      "safeBrowsingEnableExtendedReporting",
      safe_browsing::ChooseOptInTextResource(
          *profile->GetPrefs(),
          IDS_SETTINGS_SAFEBROWSING_ENABLE_EXTENDED_REPORTING,
          IDS_SETTINGS_SAFEBROWSING_ENABLE_SCOUT_REPORTING));
  html_source->AddString(
      "improveBrowsingExperience",
      l10n_util::GetStringFUTF16(
          IDS_SETTINGS_IMPROVE_BROWSING_EXPERIENCE,
#if defined(OS_CHROMEOS)
          GetHelpUrlWithBoard(chrome::kPrivacyLearnMoreURL)));
#else
          base::ASCIIToUTF16(chrome::kPrivacyLearnMoreURL)));
#endif
  html_source->AddString(
      "doNotTrackDialogMessage",
      l10n_util::GetStringFUTF16(
          IDS_SETTINGS_ENABLE_DO_NOT_TRACK_DIALOG_TEXT,
#if defined(OS_CHROMEOS)
          GetHelpUrlWithBoard(chrome::kDoNotTrackLearnMoreURL)));
#else
          base::ASCIIToUTF16(chrome::kDoNotTrackLearnMoreURL)));
#endif
  html_source->AddString(
      "exceptionsLearnMoreURL",
      base::ASCIIToUTF16(chrome::kContentSettingsExceptionsLearnMoreURL));
}
