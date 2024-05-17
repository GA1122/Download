 bool DataReductionProxySettings::IsDataSaverEnabledByUser() const {
  
// bool DataReductionProxySettings::IsDataSaverEnabledByUser(PrefService* prefs) {
    if (params::ShouldForceEnableDataReductionProxy())
      return true;
  
  if (spdy_proxy_auth_enabled_.GetPrefName().empty())
    return false;
  return spdy_proxy_auth_enabled_.GetValue();
//   return prefs && prefs->GetBoolean(prefs::kDataSaverEnabled);
  }