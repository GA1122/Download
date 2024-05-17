LocalCardMigrationExperimentalFlag GetLocalCardMigrationExperimentalFlag() {
  if (!base::FeatureList::IsEnabled(kAutofillCreditCardLocalCardMigration))
    return LocalCardMigrationExperimentalFlag::kMigrationDisabled;

  std::string param = base::GetFieldTrialParamValueByFeature(
      kAutofillCreditCardLocalCardMigration,
      kAutofillCreditCardLocalCardMigrationParameterName);

  if (param ==
      kAutofillCreditCardLocalCardMigrationParameterWithoutSettingsPage) {
    return LocalCardMigrationExperimentalFlag::kMigrationWithoutSettingsPage;
  }
  return LocalCardMigrationExperimentalFlag::kMigrationIncludeSettingsPage;
}
