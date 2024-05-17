void BrowserEventRouter::RegisterForBrowserNotifications(Browser* browser) {
  if (!profile_->IsSameProfile(browser->profile()))
    return;
  browser->tab_strip_model()->AddObserver(this);

  for (int i = 0; i < browser->tab_strip_model()->count(); ++i) {
    RegisterForTabNotifications(chrome::GetWebContentsAt(browser, i));
  }
}
