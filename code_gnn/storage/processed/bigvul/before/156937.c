void NavigationRequest::OnRequestFailedInternal(
    bool has_stale_copy_in_cache,
    int net_error,
    const base::Optional<net::SSLInfo>& ssl_info,
    bool skip_throttles,
    const base::Optional<std::string>& error_page_content) {
  DCHECK(state_ == STARTED || state_ == RESPONSE_STARTED);
  DCHECK(!(net_error == net::ERR_ABORTED && error_page_content.has_value()));

  TRACE_EVENT_ASYNC_STEP_INTO1("navigation", "NavigationRequest", this,
                               "OnRequestFailed", "error", net_error);
  state_ = FAILED;
  if (navigation_handle_.get())
    navigation_handle_->set_net_error_code(static_cast<net::Error>(net_error));

  int expected_pending_entry_id = nav_entry_id_;
  bool is_download = false;
  if (navigation_handle_.get()) {
    expected_pending_entry_id = navigation_handle_->pending_nav_entry_id();
    is_download = navigation_handle_->IsDownload();
  }

  frame_tree_node_->navigator()->DiscardPendingEntryIfNeeded(
      expected_pending_entry_id, is_download);

  if (net_error == net::ERR_ABORTED) {
    frame_tree_node_->ResetNavigationRequest(false, true);
    return;
  }

  RenderFrameHostImpl* render_frame_host = nullptr;
  if (net_error == net::ERR_BLOCKED_BY_CLIENT && !browser_initiated()) {
    render_frame_host = frame_tree_node_->current_frame_host();
  } else {
    render_frame_host =
        frame_tree_node_->render_manager()->GetFrameHostForNavigation(*this);
  }
  DCHECK(render_frame_host);

  DCHECK(render_frame_host->CanCommitURL(common_params_.url));

  NavigatorImpl::CheckWebUIRendererDoesNotDisplayNormalURL(render_frame_host,
                                                           common_params_.url);

  has_stale_copy_in_cache_ = has_stale_copy_in_cache;
  net_error_ = net_error;

  if (skip_throttles) {
    CommitErrorPage(render_frame_host, error_page_content);
  } else {
    navigation_handle_->WillFailRequest(
        ssl_info, base::Bind(&NavigationRequest::OnFailureChecksComplete,
                             base::Unretained(this), render_frame_host));
  }
}
