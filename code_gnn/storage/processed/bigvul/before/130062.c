void VariationsService::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterStringPref(prefs::kVariationsRestrictParameter,
                               std::string());
}
