void VariationsService::RegisterPrefs(PrefRegistrySimple* registry) {
  VariationsSeedStore::RegisterPrefs(registry);
  registry->RegisterInt64Pref(prefs::kVariationsLastFetchTime, 0);
  registry->RegisterStringPref(prefs::kVariationsRestrictParameter,
                               std::string());
  registry->RegisterListPref(prefs::kVariationsPermanentConsistencyCountry);
}
