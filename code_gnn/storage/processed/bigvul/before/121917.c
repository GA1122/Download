bool ChromeNetworkDelegate::OnCanThrottleRequest(
    const net::URLRequest& request) const {
  if (g_never_throttle_requests_) {
    return false;
  }

  return request.first_party_for_cookies().scheme() ==
      extensions::kExtensionScheme;
}
