std::string ContextualSearchFieldTrial::GetResolverURLPrefix() {
  if (!is_resolver_url_prefix_cached_) {
    is_resolver_url_prefix_cached_ = true;
    resolver_url_prefix_ = GetSwitch(kContextualSearchResolverUrl);
    if (resolver_url_prefix_.empty())
      resolver_url_prefix_ = GetParam(kContextualSearchResolverUrl);
  }
  return resolver_url_prefix_;
}
