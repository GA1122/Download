void BrowserInit::LaunchWithProfile::ProcessLaunchURLs(
    bool process_startup,
    const std::vector<GURL>& urls_to_open) {
  if (process_startup && command_line_.HasSwitch(switches::kNoStartupWindow))
    return;

  if (process_startup && ProcessStartupURLs(urls_to_open)) {
    return;
  }

  if (!process_startup) {
    SessionService* service = SessionServiceFactory::GetForProfile(profile_);
    if (service && service->ShouldNewWindowStartSession()) {
      if (service->RestoreIfNecessary(urls_to_open))
        return;
      if (ProcessSpecifiedURLs(urls_to_open))
        return;
    }
  }


  Browser* browser = NULL;
  std::vector<GURL> adjust_urls = urls_to_open;
  if (adjust_urls.empty())
    AddStartupURLs(&adjust_urls);
  else if (!command_line_.HasSwitch(switches::kOpenInNewWindow))
    browser = BrowserList::GetLastActiveWithProfile(profile_);

  browser = OpenURLsInBrowser(browser, process_startup, adjust_urls);
  if (process_startup)
    AddInfoBarsIfNecessary(browser);
}
