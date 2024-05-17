bool MetricsWebContentsObserver::ShouldTrackNavigation(
    content::NavigationHandle* navigation_handle) const {
  DCHECK(navigation_handle->IsInMainFrame());
  DCHECK(!navigation_handle->HasCommitted() ||
         !navigation_handle->IsSameDocument());

  return BrowserPageTrackDecider(embedder_interface_.get(), navigation_handle)
      .ShouldTrack();
}
