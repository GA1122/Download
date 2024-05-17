void ExtensionSettingsHandler::RegisterUserPrefs(PrefService* prefs) {
  prefs->RegisterBooleanPref(prefs::kExtensionsUIDeveloperMode,
                             false,
                             PrefService::SYNCABLE_PREF);
}
