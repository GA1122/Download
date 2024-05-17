void MetricsWebContentsObserver::WillStartNavigationRequest(
    content::NavigationHandle* navigation_handle) {
  DCHECK(!navigation_handle->IsSameDocument());

  if (!navigation_handle->IsInMainFrame())
    return;

  UserInitiatedInfo user_initiated_info(
      CreateUserInitiatedInfo(navigation_handle, committed_load_.get()));
  std::unique_ptr<PageLoadTracker> last_aborted =
      NotifyAbortedProvisionalLoadsNewNavigation(navigation_handle,
                                                 user_initiated_info);

  int chain_size_same_url = 0;
  int chain_size = 0;
  if (last_aborted) {
    if (last_aborted->MatchesOriginalNavigation(navigation_handle)) {
      chain_size_same_url = last_aborted->aborted_chain_size_same_url() + 1;
    } else if (last_aborted->aborted_chain_size_same_url() > 0) {
      LogAbortChainSameURLHistogram(
          last_aborted->aborted_chain_size_same_url());
    }
    chain_size = last_aborted->aborted_chain_size() + 1;
  }

  if (!ShouldTrackNavigation(navigation_handle))
    return;

  content::RenderFrameHost* opener = web_contents()->GetOpener();
  const GURL& opener_url = !has_navigated_ && opener
                               ? opener->GetLastCommittedURL()
                               : GURL::EmptyGURL();
  const GURL& currently_committed_url =
      committed_load_ ? committed_load_->url() : opener_url;
  has_navigated_ = true;

  DCHECK_GT(2ul, provisional_loads_.size());
  auto insertion_result = provisional_loads_.insert(std::make_pair(
      navigation_handle,
      std::make_unique<PageLoadTracker>(
          in_foreground_, embedder_interface_.get(), currently_committed_url,
          navigation_handle, user_initiated_info, chain_size,
          chain_size_same_url)));
  DCHECK(insertion_result.second)
      << "provisional_loads_ already contains NavigationHandle.";
  for (auto& observer : testing_observers_)
    observer.OnTrackerCreated(insertion_result.first->second.get());
}
