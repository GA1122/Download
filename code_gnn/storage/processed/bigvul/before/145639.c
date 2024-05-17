bool NavigationRequest::ShouldKeepErrorPageInCurrentProcess(int net_error) {
  return net_error == net::ERR_BLOCKED_BY_CLIENT && !browser_initiated();
}
