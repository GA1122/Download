void MetricsWebContentsObserver::HandleCommittedNavigationForTrackedLoad(
    content::NavigationHandle* navigation_handle,
    std::unique_ptr<PageLoadTracker> tracker) {
  if (!IsNavigationUserInitiated(navigation_handle) &&
      (navigation_handle->GetPageTransition() &
       ui::PAGE_TRANSITION_CLIENT_REDIRECT) != 0 &&
      committed_load_) {
    committed_load_->NotifyClientRedirectTo(*tracker);
  }

  committed_load_ = std::move(tracker);
  committed_load_->Commit(navigation_handle);
  DCHECK(committed_load_->did_commit());

  for (auto& observer : testing_observers_)
    observer.OnCommit(committed_load_.get());
}
