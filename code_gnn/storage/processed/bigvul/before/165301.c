bool ChromeContentBrowserClient::IsDataSaverEnabled(
    content::BrowserContext* browser_context) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (!profile)
    return false;
  PrefService* prefs = profile->GetPrefs();
  return prefs && prefs->GetBoolean(prefs::kDataSaverEnabled);
}
