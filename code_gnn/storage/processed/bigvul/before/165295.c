bool GetDataSaverEnabledPref(const PrefService* prefs) {
  return prefs->GetBoolean(prefs::kDataSaverEnabled) &&
         base::FieldTrialList::FindFullName("SaveDataHeader")
             .compare("Disabled");
}
