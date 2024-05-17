ResourceFetcher::DetermineRevalidationPolicy(
    Resource::Type type,
    const FetchParameters& fetch_params,
    const Resource& existing_resource,
    bool is_static_data) const {
  RevalidationPolicy policy = DetermineRevalidationPolicyInternal(
      type, fetch_params, existing_resource, is_static_data);

  TRACE_EVENT_INSTANT1("blink", "ResourceFetcher::DetermineRevalidationPolicy",
                       TRACE_EVENT_SCOPE_THREAD, "revalidationPolicy", policy);

  return policy;
}
