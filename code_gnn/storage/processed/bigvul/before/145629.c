void NavigationRequest::OnRedirectChecksComplete(
    NavigationThrottle::ThrottleCheckResult result) {
  DCHECK(result.action() != NavigationThrottle::DEFER);
  DCHECK(result.action() != NavigationThrottle::BLOCK_RESPONSE);

  bool collapse_frame =
      result.action() == NavigationThrottle::BLOCK_REQUEST_AND_COLLAPSE;

  if (result.action() == NavigationThrottle::CANCEL_AND_IGNORE ||
      result.action() == NavigationThrottle::CANCEL) {
    DCHECK(result.action() == NavigationThrottle::CANCEL ||
           result.net_error_code() == net::ERR_ABORTED);
    OnRequestFailedInternal(
        network::URLLoaderCompletionStatus(result.net_error_code()),
        true  , result.error_page_content(), collapse_frame);

    return;
  }

  if (result.action() == NavigationThrottle::BLOCK_REQUEST ||
      result.action() == NavigationThrottle::BLOCK_REQUEST_AND_COLLAPSE) {
    DCHECK(result.net_error_code() == net::ERR_BLOCKED_BY_CLIENT ||
           result.net_error_code() == net::ERR_BLOCKED_BY_ADMINISTRATOR);
    OnRequestFailedInternal(
        network::URLLoaderCompletionStatus(result.net_error_code()),
        true  , result.error_page_content(), collapse_frame);
    return;
  }

  RenderFrameDevToolsAgentHost::OnNavigationRequestWillBeSent(*this);

  base::Optional<net::HttpRequestHeaders> embedder_additional_headers;
  GetContentClient()->browser()->NavigationRequestRedirected(
      frame_tree_node_->frame_tree_node_id(), common_params_.url,
      &embedder_additional_headers);

  loader_->FollowRedirect(base::nullopt,
                          std::move(embedder_additional_headers));
}
