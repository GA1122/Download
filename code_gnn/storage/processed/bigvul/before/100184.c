Browser* BrowserInit::LaunchWithProfile::OpenURLsInBrowser(
    Browser* browser,
    bool process_startup,
    const std::vector<GURL>& urls) {
  DCHECK(!urls.empty());
  if (!profile_ && browser)
    profile_ = browser->profile();

  int pin_count = 0;
  if (!browser) {
    std::string pin_count_string =
        command_line_.GetSwitchValueASCII(switches::kPinnedTabCount);
    if (!pin_count_string.empty())
      pin_count = StringToInt(pin_count_string);
  }
  if (!browser || browser->type() != Browser::TYPE_NORMAL)
    browser = Browser::Create(profile_);

#if !defined(OS_MACOSX)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode))
    browser->ToggleFullscreenMode();
#endif

  for (size_t i = 0; i < urls.size(); ++i) {
    if (!process_startup && !URLRequest::IsHandledURL(urls[i]))
      continue;
    TabContents* tab = browser->AddTabWithURL(
        urls[i], GURL(), PageTransition::START_PAGE, (i == 0), -1, false, NULL);
    if (i < static_cast<size_t>(pin_count))
      browser->tabstrip_model()->SetTabPinned(browser->tab_count() - 1, true);
    if (profile_ && i == 0 && process_startup)
      AddCrashedInfoBarIfNecessary(tab);
  }
  browser->window()->Show();
  browser->GetSelectedTabContents()->view()->SetInitialFocus();

  return browser;
}
