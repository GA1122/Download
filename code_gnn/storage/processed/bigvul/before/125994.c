void NTPInfoObserver::Observe(int type,
                              const content::NotificationSource& source,
                              const content::NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_TOP_SITES_LOADED) {
    OnTopSitesLoaded();
  } else if (type == chrome::NOTIFICATION_TOP_SITES_UPDATED) {
    content::Details<CancelableRequestProvider::Handle> request_details(
          details);
    if (request_ == *request_details.ptr()) {
      top_sites_->GetMostVisitedURLs(
          base::Bind(&NTPInfoObserver::OnTopSitesReceived,
                     base::Unretained(this)));
    }
  }
}
