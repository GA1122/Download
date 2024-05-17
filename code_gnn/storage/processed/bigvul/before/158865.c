void Browser::TabClosingAt(TabStripModel* tab_strip_model,
                           WebContents* contents,
                           int index) {
  WebContentsModalDialogManager::FromWebContents(contents)->CloseAllDialogs();

  page_load_metrics::MetricsWebContentsObserver* metrics_observer =
      page_load_metrics::MetricsWebContentsObserver::FromWebContents(contents);
  metrics_observer->WebContentsWillSoonBeDestroyed();

  exclusive_access_manager_->OnTabClosing(contents);
  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_);
  if (session_service)
    session_service->TabClosing(contents);
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_TAB_CLOSING,
      content::Source<NavigationController>(&contents->GetController()),
      content::NotificationService::NoDetails());
}
