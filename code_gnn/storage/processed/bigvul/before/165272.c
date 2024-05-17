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

  devtools_instrumentation::OnNavigationRequestWillBeSent(*this);

  net::HttpRequestHeaders modified_headers =
      navigation_handle_->TakeModifiedRequestHeaders();
  std::vector<std::string> removed_headers =
      navigation_handle_->TakeRemovedRequestHeaders();

  BrowserContext* browser_context =
      frame_tree_node_->navigator()->GetController()->GetBrowserContext();
  if (browser_context->GetClientHintsControllerDelegate()) {
    net::HttpRequestHeaders client_hints_extra_headers;
    browser_context->GetClientHintsControllerDelegate()
        ->GetAdditionalNavigationRequestClientHintsHeaders(
            common_params_.url, &client_hints_extra_headers);
    modified_headers.MergeFrom(client_hints_extra_headers);
  }

  loader_->FollowRedirect(std::move(removed_headers),
                          std::move(modified_headers),
                          common_params_.previews_state);
}
