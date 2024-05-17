void OneClickSigninSyncStarter::EnsureBrowser() {
  if (!browser_) {
    browser_ = chrome::FindLastActiveWithProfile(profile_, desktop_type_);
    if (!browser_) {
      browser_ = new Browser(Browser::CreateParams(profile_,
                                                   desktop_type_));
      chrome::AddBlankTabAt(browser_, -1, true);
    }
    browser_->window()->Show();
  }
}
