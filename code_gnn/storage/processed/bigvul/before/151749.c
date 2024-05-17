void Browser::TabClosingAt(TabStripModel* tab_strip_model,
                           WebContents* contents,
                           int index) {
  exclusive_access_manager_->OnTabClosing(contents);
  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_);
  if (session_service)
    session_service->TabClosing(contents);
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_TAB_CLOSING,
      content::Source<NavigationController>(&contents->GetController()),
      content::NotificationService::NoDetails());

  SetAsDelegate(contents, false);
}
