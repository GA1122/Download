Resource* ResourceFetcher::CreateResourceForLoading(
    FetchParameters& params,
    const ResourceFactory& factory) {
  const String cache_identifier = GetCacheIdentifier();
  DCHECK(!IsMainThread() ||
         !GetMemoryCache()->ResourceForURL(params.GetResourceRequest().Url(),
                                           cache_identifier));

  RESOURCE_LOADING_DVLOG(1) << "Loading Resource for "
                            << params.GetResourceRequest().Url().ElidedString();

  Resource* resource = factory.Create(
      params.GetResourceRequest(), params.Options(), params.DecoderOptions());
  resource->SetLinkPreload(params.IsLinkPreload());
  if (params.IsSpeculativePreload()) {
    resource->SetPreloadDiscoveryTime(params.PreloadDiscoveryTime());
  }
  resource->SetCacheIdentifier(cache_identifier);

  if (ShouldResourceBeAddedToMemoryCache(params, resource))
    GetMemoryCache()->Add(resource);
  return resource;
}
