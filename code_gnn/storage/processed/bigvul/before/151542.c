WebCachePolicy FetchContext::ResourceRequestCachePolicy(
    const ResourceRequest&,
    Resource::Type,
    FetchParameters::DeferOption defer) const {
  return WebCachePolicy::kUseProtocolCachePolicy;
}
