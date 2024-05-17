void TriggerShowLoginWizardFinish(
    std::string switch_locale,
    std::unique_ptr<ShowLoginWizardSwitchLanguageCallbackData> data) {
  if (switch_locale.empty()) {
    ShowLoginWizardFinish(data->first_screen, data->startup_manifest);
  } else {
    chromeos::locale_util::SwitchLanguageCallback callback(
        base::Bind(&OnLanguageSwitchedCallback, base::Passed(std::move(data))));

    chromeos::locale_util::SwitchLanguage(
        switch_locale, true, true  , callback,
        ProfileManager::GetActiveUserProfile());
  }
}
