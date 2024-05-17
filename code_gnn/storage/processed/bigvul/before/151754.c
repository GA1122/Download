void Browser::TabPinnedStateChanged(TabStripModel* tab_strip_model,
                                    WebContents* contents,
                                    int index) {
  SessionService* session_service =
      SessionServiceFactory::GetForProfileIfExisting(profile());
  if (session_service) {
    SessionTabHelper* session_tab_helper =
        SessionTabHelper::FromWebContents(contents);
    session_service->SetPinnedState(session_id(),
                                    session_tab_helper->session_id(),
                                    tab_strip_model_->IsTabPinned(index));
  }
}
