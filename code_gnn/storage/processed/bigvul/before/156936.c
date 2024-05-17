void NavigationRequest::OnRequestFailed(
    bool has_stale_copy_in_cache,
    int net_error,
    const base::Optional<net::SSLInfo>& ssl_info) {
  RenderFrameDevToolsAgentHost::OnNavigationRequestFailed(*this, net_error);

  NavigationRequest::OnRequestFailedInternal(has_stale_copy_in_cache, net_error,
                                             ssl_info, false, base::nullopt);
}
