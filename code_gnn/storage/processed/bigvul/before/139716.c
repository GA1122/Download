Resource* ResourceFetcher::CreateResourceForLoading(
    FetchRequest& request,
    const String& charset,
    const ResourceFactory& factory) {
  const String cache_identifier = GetCacheIdentifier();
  DCHECK(!GetMemoryCache()->ResourceForURL(request.GetResourceRequest().Url(),
                                           cache_identifier));

  RESOURCE_LOADING_DVLOG(1)
      << "Loading Resource for "
      << request.GetResourceRequest().Url().ElidedString();

  Resource* resource =
      factory.Create(request.GetResourceRequest(), request.Options(), charset);
  resource->SetLinkPreload(request.IsLinkPreload());
  if (request.IsSpeculativePreload()) {
    resource->SetPreloadDiscoveryTime(request.PreloadDiscoveryTime());
  }
  resource->SetCacheIdentifier(cache_identifier);

  if (factory.GetType() != Resource::kMainResource &&
      request.Options().data_buffering_policy != kDoNotBufferData) {
    GetMemoryCache()->Add(resource);
  }
  return resource;
}
