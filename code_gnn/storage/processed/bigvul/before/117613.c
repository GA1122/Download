void BrowserInit::LaunchWithProfile::AddStartupURLs(
    std::vector<GURL>* startup_urls) const {
  if (!startup_urls->empty())
    return;

  if (browser_init_) {
    if (!browser_init_->first_run_tabs_.empty()) {
      std::vector<GURL>::iterator it = browser_init_->first_run_tabs_.begin();
      while (it != browser_init_->first_run_tabs_.end()) {
        if (it->host() == "new_tab_page") {
          startup_urls->push_back(GURL(chrome::kChromeUINewTabURL));
        } else if (it->host() == "welcome_page") {
          startup_urls->push_back(GetWelcomePageURL());
        } else {
          startup_urls->push_back(*it);
        }
        ++it;
      }
      browser_init_->first_run_tabs_.clear();
    }
  }

  if (startup_urls->empty()) {
    startup_urls->push_back(GURL());   
    PrefService* prefs = g_browser_process->local_state();
    if (prefs->FindPreference(prefs::kShouldShowWelcomePage) &&
        prefs->GetBoolean(prefs::kShouldShowWelcomePage)) {
      prefs->ClearPref(prefs::kShouldShowWelcomePage);
      startup_urls->push_back(GetWelcomePageURL());
    }
  }

  if (SyncPromoUI::ShouldShowSyncPromoAtStartup(profile_, is_first_run_)) {
    SyncPromoUI::DidShowSyncPromoAtStartup(profile_);
    GURL old_url = (*startup_urls)[0];
    (*startup_urls)[0] =
        SyncPromoUI::GetSyncPromoURL(GURL(chrome::kChromeUINewTabURL), true);

    if (old_url.is_empty() &&
        profile_->GetHomePage() == GURL(chrome::kChromeUINewTabURL)) {
      old_url = GURL(chrome::kChromeUINewTabURL);
    }

    if (old_url != GURL(chrome::kChromeUINewTabURL))
      startup_urls->insert(startup_urls->begin() + 1, old_url);

    if (startup_urls->size() > 2) {
      std::vector<GURL>::iterator it = std::find(
          startup_urls->begin(), startup_urls->end(), GetWelcomePageURL());
      if (it != startup_urls->end())
        startup_urls->erase(it);
    }
  }
}
