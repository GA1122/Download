void BrowserInit::LaunchWithProfile::CheckDefaultBrowser(Profile* profile) {
  if (!profile->GetPrefs()->GetBoolean(prefs::kCheckDefaultBrowser) ||
      is_first_run_) {
    return;
  }
  if (g_browser_process->local_state()->IsManagedPreference(
      prefs::kDefaultBrowserSettingEnabled)) {
    if (g_browser_process->local_state()->GetBoolean(
        prefs::kDefaultBrowserSettingEnabled)) {
      BrowserThread::PostTask(
          BrowserThread::FILE, FROM_HERE,
          base::Bind(
              base::IgnoreResult(&ShellIntegration::SetAsDefaultBrowser)));
    } else {
    }
    return;
  }
  BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE,
                          base::Bind(&CheckDefaultBrowserCallback));
}
