mojom::NavigationClient* NavigationRequest::GetCommitNavigationClient() {
  if (commit_navigation_client_ && commit_navigation_client_.is_bound())
    return commit_navigation_client_.get();
  commit_navigation_client_ =
      render_frame_host_->GetNavigationClientFromInterfaceProvider();
  return commit_navigation_client_.get();
}
