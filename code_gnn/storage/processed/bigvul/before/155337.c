ChromeContentBrowserClient::GetDevToolsBackgroundServiceExpirations(
    content::BrowserContext* browser_context) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  DCHECK(profile);

  auto* pref_service = profile->GetPrefs();
  DCHECK(pref_service);

  auto* expiration_dict = pref_service->GetDictionary(
      prefs::kDevToolsBackgroundServicesExpirationDict);
  DCHECK(expiration_dict);

  base::flat_map<int, base::Time> expiration_times;
  for (const auto& it : *expiration_dict) {
    int service = 0;
    bool did_convert = base::StringToInt(it.first, &service);
    DCHECK(did_convert);

    DCHECK(it.second->is_int());
    base::TimeDelta delta = base::TimeDelta::FromMinutes(it.second->GetInt());
    base::Time expiration_time = base::Time::FromDeltaSinceWindowsEpoch(delta);

    expiration_times[service] = expiration_time;
  }

  return expiration_times;
}
