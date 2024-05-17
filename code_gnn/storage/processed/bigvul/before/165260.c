void NavigationRequest::CommitErrorPage(
    const base::Optional<std::string>& error_page_content) {
  UpdateCommitNavigationParamsHistory();
  frame_tree_node_->TransferNavigationRequestOwnership(render_frame_host_);
  commit_params_.origin_to_commit.reset();
  if (IsPerNavigationMojoInterfaceEnabled() && request_navigation_client_ &&
      request_navigation_client_.is_bound()) {
    if (associated_site_instance_id_ ==
        render_frame_host_->GetSiteInstance()->GetId()) {
      commit_navigation_client_ = std::move(request_navigation_client_);
    } else {
      IgnoreInterfaceDisconnection();
    }
    associated_site_instance_id_.reset();
  }

  navigation_handle_->ReadyToCommitNavigation(true);
  render_frame_host_->FailedNavigation(this, common_params_, commit_params_,
                                       has_stale_copy_in_cache_, net_error_,
                                       error_page_content);
}
