void NavigationRequest::OnFailureChecksComplete(
    NavigationThrottle::ThrottleCheckResult result) {
  DCHECK(result.action() != NavigationThrottle::DEFER);

  int old_net_error = net_error_;
  net_error_ = result.net_error_code();
  navigation_handle_->set_net_error_code(static_cast<net::Error>(net_error_));

  if (net::ERR_ABORTED == result.net_error_code()) {
    frame_tree_node_->ResetNavigationRequest(false, true);
    return;
  }

  if (!SiteIsolationPolicy::IsErrorPageIsolationEnabled(
          frame_tree_node_->IsMainFrame())) {
    CHECK_EQ(ShouldKeepErrorPageInCurrentProcess(old_net_error),
             ShouldKeepErrorPageInCurrentProcess(net_error_))
        << " Unsupported error code change in WillFailRequest(): from "
        << net_error_ << " to " << result.net_error_code();
  }

  CommitErrorPage(result.error_page_content());
}
