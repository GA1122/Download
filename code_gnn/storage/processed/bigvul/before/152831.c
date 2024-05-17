void MetricsWebContentsObserver::HandleFailedNavigationForTrackedLoad(
    content::NavigationHandle* navigation_handle,
    std::unique_ptr<PageLoadTracker> tracker) {
  const base::TimeTicks now = base::TimeTicks::Now();
  tracker->FailedProvisionalLoad(navigation_handle, now);

  const net::Error error = navigation_handle->GetNetErrorCode();

  const bool is_aborted_provisional_load =
      error == net::OK || error == net::ERR_ABORTED;

  tracker->NotifyPageEnd(
      is_aborted_provisional_load ? END_OTHER : END_PROVISIONAL_LOAD_FAILED,
      UserInitiatedInfo::NotUserInitiated(), now, true);

  if (is_aborted_provisional_load)
    aborted_provisional_loads_.push_back(std::move(tracker));
}
