bool Resource::CanReuseRedirectChain() const {
  for (auto& redirect : redirect_chain_) {
    if (!CanUseResponse(redirect.redirect_response_, response_timestamp_))
      return false;
    if (redirect.request_.CacheControlContainsNoCache() ||
        redirect.request_.CacheControlContainsNoStore())
      return false;
  }
  return true;
}
