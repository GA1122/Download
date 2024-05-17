  void NotifySessionServiceOfRestoredTabs(Browser* browser, int initial_count) {
    SessionService* session_service =
        SessionServiceFactory::GetForProfile(profile_);
    for (int i = initial_count; i < browser->tab_count(); ++i)
      session_service->TabRestored(browser->GetTabContentsWrapperAt(i),
                                   browser->IsTabPinned(i));
  }
