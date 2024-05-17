void ResourceFetcher::InsertAsPreloadIfNecessary(Resource* resource,
                                                 const FetchParameters& params,
                                                 Resource::Type type) {
  if (!params.IsSpeculativePreload() && !params.IsLinkPreload())
    return;
  if (resource->GetResourceError().IsAccessCheck())
    return;
  PreloadKey key(params.Url(), type);
  if (preloads_.find(key) == preloads_.end()) {
    preloads_.insert(key, resource);
    resource->MarkAsPreload();
    if (preloaded_urls_for_test_)
      preloaded_urls_for_test_->insert(resource->Url().GetString());
  }
}
