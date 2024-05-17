void MetricsWebContentsObserver::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  if (!navigation_handle->IsInMainFrame()) {
    if (committed_load_ && navigation_handle->GetParentFrame() &&
        GetMainFrame(navigation_handle->GetParentFrame()) ==
            web_contents()->GetMainFrame()) {
      committed_load_->DidFinishSubFrameNavigation(navigation_handle);
      committed_load_->metrics_update_dispatcher()->DidFinishSubFrameNavigation(
          navigation_handle);
    }
    return;
  }

  std::unique_ptr<PageLoadTracker> finished_nav(
      std::move(provisional_loads_[navigation_handle]));
  provisional_loads_.erase(navigation_handle);

  if (navigation_handle->HasCommitted() &&
      navigation_handle->IsSameDocument()) {
    if (finished_nav)
      finished_nav->StopTracking();
    if (committed_load_)
      committed_load_->DidCommitSameDocumentNavigation(navigation_handle);
    return;
  }

  if (!navigation_handle->HasCommitted() &&
      navigation_handle->GetNetErrorCode() == net::ERR_ABORTED &&
      navigation_handle->GetResponseHeaders()) {
    if (finished_nav) {
      finished_nav->DidInternalNavigationAbort(navigation_handle);
      finished_nav->StopTracking();
    }
    return;
  }

  const bool should_track =
      finished_nav && ShouldTrackNavigation(navigation_handle);

  if (finished_nav && !should_track)
    finished_nav->StopTracking();

  if (navigation_handle->HasCommitted()) {
    UserInitiatedInfo user_initiated_info =
        finished_nav
            ? finished_nav->user_initiated_info()
            : CreateUserInitiatedInfo(navigation_handle, committed_load_.get());

    NotifyPageEndAllLoadsWithTimestamp(
        EndReasonForPageTransition(navigation_handle->GetPageTransition()),
        user_initiated_info, navigation_handle->NavigationStart(), false);

    if (should_track) {
      HandleCommittedNavigationForTrackedLoad(navigation_handle,
                                              std::move(finished_nav));
    } else {
      committed_load_.reset();
    }
  } else if (should_track) {
    HandleFailedNavigationForTrackedLoad(navigation_handle,
                                         std::move(finished_nav));
  }
}
