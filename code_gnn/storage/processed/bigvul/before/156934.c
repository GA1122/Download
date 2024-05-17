void NavigationRequest::OnFailureChecksComplete(
    RenderFrameHostImpl* render_frame_host,
    NavigationThrottle::ThrottleCheckResult result) {
  DCHECK(result.action() != NavigationThrottle::DEFER);

  net_error_ = result.net_error_code();
  navigation_handle_->set_net_error_code(static_cast<net::Error>(net_error_));

  if (net::ERR_ABORTED == net_error_) {
    frame_tree_node_->ResetNavigationRequest(false, true);
    return;
  }

  CommitErrorPage(render_frame_host, result.error_page_content());
}
