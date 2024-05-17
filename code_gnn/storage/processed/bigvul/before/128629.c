std::string ContextualSearchDelegate::GetAcceptLanguages() {
  Profile* profile = ProfileManager::GetActiveUserProfile();
  PrefService* pref_service = profile->GetPrefs();
  return pref_service->GetString(prefs::kAcceptLanguages);
}
