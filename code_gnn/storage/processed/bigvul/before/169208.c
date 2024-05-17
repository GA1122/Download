void NavigationHandleCommitObserver::DidFinishNavigation(
    content::NavigationHandle* handle) {
  if (handle->GetURL() != url_)
    return;
  has_committed_ = true;
  was_same_document_ = handle->IsSameDocument();
  was_renderer_initiated_ = handle->IsRendererInitiated();
}
