void CorePageLoadMetricsObserver::OnFailedProvisionalLoad(
    content::NavigationHandle* navigation_handle) {
  net::Error error = navigation_handle->GetNetErrorCode();
  if (error == net::OK || error == net::ERR_ABORTED) {
    return;
  }

  failed_provisional_load_info_.interval =
      base::TimeTicks::Now() - navigation_handle->NavigationStart();
  failed_provisional_load_info_.error = error;
}
