  void RestoreForeignTab(const SessionTab& tab) {
    StartTabCreation();
    Browser* current_browser =
        browser_ ? browser_ : BrowserList::GetLastActiveWithProfile(profile_);
    RestoreTab(tab, current_browser->tab_count(), current_browser, true);
    NotifySessionServiceOfRestoredTabs(current_browser,
                                       current_browser->tab_count());
    FinishedTabCreation(true, true);
  }
