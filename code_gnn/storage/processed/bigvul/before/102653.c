void CloudPolicySubsystem::RegisterPrefs(PrefService* pref_service) {
  pref_service->RegisterIntegerPref(prefs::kDevicePolicyRefreshRate,
                                    kDefaultPolicyRefreshRateMs,
                                    PrefService::UNSYNCABLE_PREF);
  pref_service->RegisterIntegerPref(prefs::kUserPolicyRefreshRate,
                                    kDefaultPolicyRefreshRateMs,
                                    PrefService::UNSYNCABLE_PREF);
}
