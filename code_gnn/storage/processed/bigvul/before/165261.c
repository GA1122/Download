void NavigationRequest::CommitNavigation() {
  UpdateCommitNavigationParamsHistory();
  DCHECK(response_ || !IsURLHandledByNetworkStack(common_params_.url) ||
         navigation_handle_->IsSameDocument());
  DCHECK(!common_params_.url.SchemeIs(url::kJavaScriptScheme));

  DCHECK(render_frame_host_ ==
             frame_tree_node_->render_manager()->current_frame_host() ||
         render_frame_host_ ==
             frame_tree_node_->render_manager()->speculative_frame_host());

  if (!frame_tree_node_->navigation_request()) {
    DEBUG_ALIAS_FOR_GURL(url, navigation_handle_->GetURL());
    base::debug::DumpWithoutCrashing();
  }

  frame_tree_node_->TransferNavigationRequestOwnership(render_frame_host_);
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
  render_frame_host_->CommitNavigation(
      this, response_.get(), std::move(url_loader_client_endpoints_),
      common_params_, commit_params_, is_view_source_,
      std::move(subresource_loader_params_), std::move(subresource_overrides_),
      devtools_navigation_token_);

  RenderProcessHostImpl::NotifySpareManagerAboutRecentlyUsedBrowserContext(
      render_frame_host_->GetSiteInstance()->GetBrowserContext());
}
