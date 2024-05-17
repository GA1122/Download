void AddEasyUnlockStrings(content::WebUIDataSource* html_source) {
  LocalizedString localized_strings[] = {
      {"easyUnlockSectionTitle", IDS_SETTINGS_EASY_UNLOCK_SECTION_TITLE},
      {"easyUnlockSetupButton", IDS_SETTINGS_EASY_UNLOCK_SETUP},
      {"easyUnlockTurnOffButton", IDS_SETTINGS_EASY_UNLOCK_TURN_OFF},
      {"easyUnlockTurnOffOfflineTitle",
       IDS_SETTINGS_EASY_UNLOCK_TURN_OFF_OFFLINE_TITLE},
      {"easyUnlockTurnOffOfflineMessage",
       IDS_SETTINGS_EASY_UNLOCK_TURN_OFF_OFFLINE_MESSAGE},
      {"easyUnlockTurnOffErrorTitle",
       IDS_SETTINGS_EASY_UNLOCK_TURN_OFF_ERROR_TITLE},
      {"easyUnlockTurnOffErrorMessage",
       IDS_SETTINGS_EASY_UNLOCK_TURN_OFF_ERROR_MESSAGE},
      {"easyUnlockTurnOffRetryButton", IDS_SETTINGS_EASY_UNLOCK_TURN_OFF_RETRY},
      {"easyUnlockAllowSignInLabel",
       IDS_SETTINGS_EASY_UNLOCK_ALLOW_SIGN_IN_LABEL},
      {"easyUnlockProximityThresholdLabel",
       IDS_SETTINGS_EASY_UNLOCK_PROXIMITY_THRESHOLD_LABEL},
      {"easyUnlockProximityThresholdVeryClose",
       IDS_SETTINGS_EASY_UNLOCK_PROXIMITY_THRESHOLD_VERY_CLOSE},
      {"easyUnlockProximityThresholdClose",
       IDS_SETTINGS_EASY_UNLOCK_PROXIMITY_THRESHOLD_CLOSE},
      {"easyUnlockProximityThresholdFar",
       IDS_SETTINGS_EASY_UNLOCK_PROXIMITY_THRESHOLD_FAR},
      {"easyUnlockProximityThresholdVeryFar",
       IDS_SETTINGS_EASY_UNLOCK_PROXIMITY_THRESHOLD_VERY_FAR},
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

  base::string16 device_name =
      l10n_util::GetStringUTF16(ui::GetChromeOSDeviceTypeResourceId());
  html_source->AddString(
      "easyUnlockSetupIntro",
      l10n_util::GetStringFUTF16(IDS_SETTINGS_EASY_UNLOCK_SETUP_INTRO,
                                 device_name));
  html_source->AddString(
      "easyUnlockDescription",
      l10n_util::GetStringFUTF16(IDS_SETTINGS_EASY_UNLOCK_DESCRIPTION,
                                 device_name));
  html_source->AddString(
      "easyUnlockTurnOffTitle",
      l10n_util::GetStringFUTF16(IDS_SETTINGS_EASY_UNLOCK_TURN_OFF_TITLE,
                                 device_name));
  html_source->AddString(
      "easyUnlockTurnOffDescription",
      l10n_util::GetStringFUTF16(IDS_SETTINGS_EASY_UNLOCK_TURN_OFF_DESCRIPTION,
                                 device_name));
  html_source->AddString(
      "easyUnlockProximityThresholdLabel",
      l10n_util::GetStringFUTF16(
          IDS_SETTINGS_EASY_UNLOCK_PROXIMITY_THRESHOLD_LABEL, device_name));

  html_source->AddString("easyUnlockLearnMoreURL",
                         GetHelpUrlWithBoard(chrome::kEasyUnlockLearnMoreUrl));
}
