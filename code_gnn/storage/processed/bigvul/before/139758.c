void ResourceFetcher::UpdateMemoryCacheStats(Resource* resource,
                                             RevalidationPolicy policy,
                                             const FetchRequest& request,
                                             const ResourceFactory& factory,
                                             bool is_static_data) const {
  if (is_static_data)
    return;

  if (request.IsSpeculativePreload() || request.IsLinkPreload()) {
    DEFINE_RESOURCE_HISTOGRAM("Preload.");
  } else {
    DEFINE_RESOURCE_HISTOGRAM("");
  }

  if (resource && !resource->IsAlive() &&
      (!preloads_ || !preloads_->Contains(resource))) {
    DEFINE_RESOURCE_HISTOGRAM("Dead.");
  }
}
