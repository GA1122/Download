ExtensionNavigationThrottle::WillRedirectRequest() {
  ThrottleCheckResult result = WillStartOrRedirectRequest();
  if (result.action() == BLOCK_REQUEST) {
    return CANCEL;
  }
  return result;
}
