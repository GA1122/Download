void BrowserInit::LaunchWithProfile::CheckDefaultBrowser(Profile* profile) {
  if (!profile->GetPrefs()->GetBoolean(prefs::kCheckDefaultBrowser) ||
      FirstRun::IsChromeFirstRun()) {
    return;
  }
  ChromeThread::PostTask(
      ChromeThread::FILE, FROM_HERE, new CheckDefaultBrowserTask());
}
