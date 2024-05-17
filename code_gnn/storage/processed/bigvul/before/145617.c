void NavigationRequest::CommitNavigation() {
  UpdateRequestNavigationParamsHistory();
  DCHECK(response_ || !IsURLHandledByNetworkStack(common_params_.url) ||
         navigation_handle_->IsSameDocument());
  DCHECK(!common_params_.url.SchemeIs(url::kJavaScriptScheme));

  DCHECK(common_params_.origin_policy.empty());
  common_params_.origin_policy = navigation_handle_->origin_policy();

  RenderFrameHostImpl* render_frame_host =
      navigation_handle_->GetRenderFrameHost();
  DCHECK(render_frame_host ==
             frame_tree_node_->render_manager()->current_frame_host() ||
         render_frame_host ==
             frame_tree_node_->render_manager()->speculative_frame_host());

  frame_tree_node_->TransferNavigationRequestOwnership(render_frame_host);
  if (IsPerNavigationMojoInterfaceEnabled() && request_navigation_client_ &&
      request_navigation_client_.is_bound()) {
    IgnorePipeDisconnection();
    if (associated_site_instance_id_ ==
        render_frame_host->GetSiteInstance()->GetId()) {
      commit_navigation_client_ = std::move(request_navigation_client_);
    }
    associated_site_instance_id_.reset();
  }
  render_frame_host->CommitNavigation(
      navigation_handle_->GetNavigationId(), response_.get(),
      std::move(url_loader_client_endpoints_), common_params_, request_params_,
      is_view_source_, std::move(subresource_loader_params_),
      std::move(subresource_overrides_), devtools_navigation_token_);

  RenderProcessHostImpl::NotifySpareManagerAboutRecentlyUsedBrowserContext(
      render_frame_host->GetSiteInstance()->GetBrowserContext());
}
