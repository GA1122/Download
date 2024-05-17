void AutofillManager::RegisterUserPrefs(PrefService* prefs) {
  prefs->RegisterBooleanPref(prefs::kAutofillEnabled,
                             true,
                             PrefService::SYNCABLE_PREF);
#if defined(OS_MACOSX)
  prefs->RegisterBooleanPref(prefs::kAutofillAuxiliaryProfilesEnabled,
                             true,
                             PrefService::SYNCABLE_PREF);
#else
  prefs->RegisterBooleanPref(prefs::kAutofillAuxiliaryProfilesEnabled,
                             false,
                             PrefService::UNSYNCABLE_PREF);
#endif
  prefs->RegisterDoublePref(prefs::kAutofillPositiveUploadRate,
                            kAutofillPositiveUploadRateDefaultValue,
                            PrefService::UNSYNCABLE_PREF);
  prefs->RegisterDoublePref(prefs::kAutofillNegativeUploadRate,
                            kAutofillNegativeUploadRateDefaultValue,
                            PrefService::UNSYNCABLE_PREF);
}
