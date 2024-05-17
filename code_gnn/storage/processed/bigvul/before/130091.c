void MasterPreferences::EnforceLegacyPreferences() {
  bool create_all_shortcuts = true;
  GetBool(installer::master_preferences::kCreateAllShortcuts,
          &create_all_shortcuts);
  if (!create_all_shortcuts) {
    distribution_->SetBoolean(
        installer::master_preferences::kDoNotCreateDesktopShortcut, true);
    distribution_->SetBoolean(
        installer::master_preferences::kDoNotCreateQuickLaunchShortcut, true);
  }

  const base::ListValue* startup_urls_list = NULL;
  if (master_dictionary_ &&
      !master_dictionary_->GetList(prefs::kURLsToRestoreOnStartup, NULL) &&
      master_dictionary_->GetList(prefs::kURLsToRestoreOnStartupOld,
                                  &startup_urls_list) &&
      startup_urls_list) {
    base::ListValue* new_startup_urls_list = startup_urls_list->DeepCopy();
    master_dictionary_->Set(prefs::kURLsToRestoreOnStartup,
                            new_startup_urls_list);
  }
}
