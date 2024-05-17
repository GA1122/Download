  bool DataReductionProxySettings::IsDataReductionProxyManaged() {
  return spdy_proxy_auth_enabled_.IsManaged();
//   const PrefService::Preference* pref =
//       GetOriginalProfilePrefs()->FindPreference(prefs::kDataSaverEnabled);
//   return pref && pref->IsManaged();
  }