void Browser::TabInsertedAt(TabStripModel* tab_strip_model,
                            WebContents* contents,
                            int index,
                            bool foreground) {
  SetAsDelegate(contents, true);

  SessionTabHelper* session_tab_helper =
      SessionTabHelper::FromWebContents(contents);
  session_tab_helper->SetWindowID(session_id());

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_TAB_PARENTED,
      content::Source<content::WebContents>(contents),
      content::NotificationService::NoDetails());

  SyncHistoryWithTabs(index);

  LoadingStateChanged(contents, true);

  interstitial_observers_.push_back(new InterstitialObserver(this, contents));

  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_);
  if (session_service) {
    session_service->TabInserted(contents);
    int new_active_index = tab_strip_model_->active_index();
    if (index < new_active_index)
      session_service->SetSelectedTabInWindow(session_id(),
                                              new_active_index);
  }
}
