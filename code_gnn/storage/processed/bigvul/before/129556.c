void CorePageLoadMetricsObserver::OnCommit(
    content::NavigationHandle* navigation_handle) {
  transition_ = navigation_handle->GetPageTransition();
  initiated_by_user_gesture_ = navigation_handle->HasUserGesture();
}
