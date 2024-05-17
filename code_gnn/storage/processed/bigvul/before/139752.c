bool ResourceFetcher::StartLoad(Resource* resource) {
  DCHECK(resource);
  DCHECK(resource->StillNeedsLoad());
  if (!Context().ShouldLoadNewResource(resource->GetType())) {
    GetMemoryCache()->Remove(resource);
    return false;
  }

  ResourceRequest request(resource->GetResourceRequest());
  ResourceResponse response;

  blink::probe::PlatformSendRequest probe(&Context(), resource->Identifier(),
                                          request, response,
                                          resource->Options().initiator_info);

  Context().DispatchWillSendRequest(resource->Identifier(), request, response,
                                    resource->Options().initiator_info);

  resource->SetResourceRequest(request);

  SecurityOrigin* source_origin = Context().GetSecurityOrigin();
  if (source_origin && source_origin->HasSuborigin())
    request.SetServiceWorkerMode(WebURLRequest::ServiceWorkerMode::kNone);

  ResourceLoader* loader = ResourceLoader::Create(this, resource);
  if (resource->ShouldBlockLoadEvent())
    loaders_.insert(loader);
  else
    non_blocking_loaders_.insert(loader);

  StorePerformanceTimingInitiatorInformation(resource);
  resource->SetFetcherSecurityOrigin(source_origin);

  loader->ActivateCacheAwareLoadingIfNeeded(request);
  loader->Start(request);
  return true;
}
