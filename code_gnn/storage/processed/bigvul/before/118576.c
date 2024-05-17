void AppShortcutManager::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(
      prefs::kAppShortcutsHaveBeenCreated, false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
}
