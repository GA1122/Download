void BrowserEventRouter::OnBrowserRemoved(Browser* browser) {
  if (!profile_->IsSameProfile(browser->profile()))
    return;

  browser->tab_strip_model()->RemoveObserver(this);
}
