void NavigationRequest::BeginNavigation() {
  DCHECK(!loader_);
  DCHECK(state_ == NOT_STARTED || state_ == WAITING_FOR_RENDERER_RESPONSE);
  TRACE_EVENT_ASYNC_STEP_INTO0("navigation", "NavigationRequest", this,
                               "BeginNavigation");

  state_ = STARTED;

#if defined(OS_ANDROID)
  base::WeakPtr<NavigationRequest> this_ptr(weak_factory_.GetWeakPtr());
  bool should_override_url_loading = false;

  if (!GetContentClient()->browser()->ShouldOverrideUrlLoading(
          frame_tree_node_->frame_tree_node_id(), browser_initiated_,
          request_params_.original_url, request_params_.original_method,
          common_params_.has_user_gesture, false,
          frame_tree_node_->IsMainFrame(), common_params_.transition,
          &should_override_url_loading)) {
    return;
  }

  if (!this_ptr)
    return;

  if (should_override_url_loading) {
    OnRequestFailedInternal(
        network::URLLoaderCompletionStatus(net::ERR_ABORTED),
        false  , base::nullopt  ,
        false  );
    return;
  }
#endif

  net::Error net_error = CheckContentSecurityPolicy(
      false  , false  ,
      false  );
  if (net_error != net::OK) {
    CreateNavigationHandle();
    OnRequestFailedInternal(network::URLLoaderCompletionStatus(net_error),
                            false  ,
                            base::nullopt  ,
                            false  );
    return;
  }

  if (CheckCredentialedSubresource() ==
          CredentialedSubresourceCheckResult::BLOCK_REQUEST ||
      CheckLegacyProtocolInSubresource() ==
          LegacyProtocolInSubresourceCheckResult::BLOCK_REQUEST) {
    CreateNavigationHandle();
    OnRequestFailedInternal(
        network::URLLoaderCompletionStatus(net::ERR_ABORTED),
        false  , base::nullopt  ,
        false  );

    return;
  }

  CreateNavigationHandle();

  if (IsURLHandledByNetworkStack(common_params_.url) &&
      !navigation_handle_->IsSameDocument()) {
    navigation_handle_->WillStartRequest(
        base::Bind(&NavigationRequest::OnStartChecksComplete,
                   base::Unretained(this)));
    return;
  }

  TRACE_EVENT_ASYNC_STEP_INTO0("navigation", "NavigationRequest", this,
                               "ResponseStarted");
  state_ = RESPONSE_STARTED;

  RenderFrameHostImpl* render_frame_host =
      frame_tree_node_->render_manager()->GetFrameHostForNavigation(*this);
  NavigatorImpl::CheckWebUIRendererDoesNotDisplayNormalURL(render_frame_host,
                                                           common_params_.url);

  navigation_handle_->ReadyToCommitNavigation(render_frame_host, false);

  CommitNavigation();
}
