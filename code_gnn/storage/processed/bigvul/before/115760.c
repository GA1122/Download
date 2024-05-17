void SafeBrowsingBlockingPage::FinishMalwareDetails(int64 delay_ms) {
  if (malware_details_ == NULL)
    return;   

  Profile* profile = Profile::FromBrowserContext(tab()->browser_context());
  const PrefService::Preference* pref =
      profile->GetPrefs()->FindPreference(prefs::kSafeBrowsingReportingEnabled);

  bool value;
  if (pref && pref->GetValue()->GetAsBoolean(&value) && value) {
    BrowserThread::PostDelayedTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(&MalwareDetails::FinishCollection, malware_details_.get()),
        delay_ms);
  }
}
