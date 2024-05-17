bool RenderFrameImpl::ShouldIgnoreCommitNavigation(
    const CommitNavigationParams& commit_params) {
  if (!browser_side_navigation_pending_ &&
      !browser_side_navigation_pending_url_.is_empty() &&
      browser_side_navigation_pending_url_ == commit_params.original_url &&
      commit_params.nav_entry_id == 0) {
    return true;
  }
  return false;
}
