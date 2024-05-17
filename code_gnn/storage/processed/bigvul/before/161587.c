void AddResetStrings(content::WebUIDataSource* html_source) {
  LocalizedString localized_strings[] = {
#if defined(OS_WIN) && defined(GOOGLE_CHROME_BUILD)
    {"resetPageTitle", safe_browsing::UserInitiatedCleanupsEnabled()
                           ? IDS_SETTINGS_RESET_AND_CLEANUP
                           : IDS_SETTINGS_RESET},
#else
    {"resetPageTitle", IDS_SETTINGS_RESET},
#endif
    {"resetTrigger", IDS_SETTINGS_RESET},
    {"resetTriggerDescription",
     IDS_SETTINGS_RESET_PROFILE_SETTINGS_DESCRIPTION},
    {"resetPageExplanation", IDS_RESET_PROFILE_SETTINGS_EXPLANATION},
    {"triggeredResetPageExplanation",
     IDS_TRIGGERED_RESET_PROFILE_SETTINGS_EXPLANATION},
    {"triggeredResetPageTitle", IDS_TRIGGERED_RESET_PROFILE_SETTINGS_TITLE},
    {"resetPageCommit", IDS_RESET_PROFILE_SETTINGS_COMMIT_BUTTON},
    {"resetPageFeedback", IDS_SETTINGS_RESET_PROFILE_FEEDBACK},
#if defined(OS_CHROMEOS)
    {"powerwashTitle", IDS_SETTINGS_FACTORY_RESET},
    {"powerwashDialogTitle", IDS_SETTINGS_FACTORY_RESET_HEADING},
    {"powerwashDialogExplanation", IDS_SETTINGS_FACTORY_RESET_WARNING},
    {"powerwashDialogButton", IDS_SETTINGS_RESTART},
    {"powerwashLearnMoreUrl", IDS_FACTORY_RESET_HELP_URL},
#endif
    {"resetAutomatedDialogTitle", IDS_SETTINGS_RESET_AUTOMATED_DIALOG_TITLE},
    {"resetProfileBannerButton", IDS_SETTINGS_RESET_BANNER_RESET_BUTTON_TEXT},
    {"resetProfileBannerDescription", IDS_SETTINGS_RESET_BANNER_TEXT},
#if defined(OS_WIN) && defined(GOOGLE_CHROME_BUILD)
    {"resetCleanupComputerTrigger",
     IDS_SETTINGS_RESET_CLEAN_UP_COMPUTER_TRIGGER},
#endif
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

  html_source->AddString("resetPageLearnMoreUrl",
                         chrome::kResetProfileSettingsLearnMoreURL);
  html_source->AddString("resetProfileBannerLearnMoreUrl",
                         chrome::kAutomaticSettingsResetLearnMoreURL);
#if defined(OS_CHROMEOS)
  html_source->AddString(
      "powerwashDescription",
      l10n_util::GetStringFUTF16(IDS_SETTINGS_FACTORY_RESET_DESCRIPTION,
                                 l10n_util::GetStringUTF16(IDS_PRODUCT_NAME)));
#endif
}
