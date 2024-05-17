std::string ContextualSearchDelegate::GetTargetLanguage() {
  Profile* profile = ProfileManager::GetActiveUserProfile();
  PrefService* pref_service = profile->GetPrefs();
  std::string result = TranslateService::GetTargetLanguage(pref_service);
  DCHECK(!result.empty());
  return result;
}
