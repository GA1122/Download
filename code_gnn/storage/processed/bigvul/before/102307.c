void ExtensionPrefs::RegisterUserPrefs(PrefService* prefs) {
  prefs->RegisterDictionaryPref(kExtensionsPref, PrefService::UNSYNCABLE_PREF);
  prefs->RegisterListPref(kExtensionToolbar, PrefService::UNSYNCABLE_PREF);
  prefs->RegisterIntegerPref(prefs::kExtensionToolbarSize,
                             -1,
                             PrefService::UNSYNCABLE_PREF);
  prefs->RegisterDictionaryPref(kExtensionsBlacklistUpdate,
                                PrefService::UNSYNCABLE_PREF);
  prefs->RegisterListPref(prefs::kExtensionInstallAllowList,
                          PrefService::UNSYNCABLE_PREF);
  prefs->RegisterListPref(prefs::kExtensionInstallDenyList,
                          PrefService::UNSYNCABLE_PREF);
  prefs->RegisterListPref(prefs::kExtensionInstallForceList,
                          PrefService::UNSYNCABLE_PREF);
  prefs->RegisterStringPref(kWebStoreLogin,
                            std::string()  ,
                            PrefService::UNSYNCABLE_PREF);
}
