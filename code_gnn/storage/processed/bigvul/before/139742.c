Resource* ResourceFetcher::RequestResource(
    FetchRequest& request,
    const ResourceFactory& factory,
    const SubstituteData& substitute_data) {
  unsigned long identifier = CreateUniqueIdentifier();
  ResourceRequest& resource_request = request.MutableResourceRequest();
  network_instrumentation::ScopedResourceLoadTracker
      scoped_resource_load_tracker(identifier, resource_request);
  SCOPED_BLINK_UMA_HISTOGRAM_TIMER("Blink.Fetch.RequestResourceTime");
  TRACE_EVENT1("blink", "ResourceFetcher::requestResource", "url",
               UrlForTraceEvent(request.Url()));

  Resource* resource = nullptr;
  ResourceRequestBlockedReason blocked_reason =
      ResourceRequestBlockedReason::kNone;

  PrepareRequestResult result = PrepareRequest(
      request, factory, substitute_data, identifier, blocked_reason);
  if (result == kAbort)
    return nullptr;
  if (result == kBlock)
    return ResourceForBlockedRequest(request, factory, blocked_reason);

  if (!request.IsSpeculativePreload()) {
    Context().RecordLoadingActivity(identifier, resource_request,
                                    factory.GetType(),
                                    request.Options().initiator_info.name);
  }

  bool is_data_url = resource_request.Url().ProtocolIsData();
  bool is_static_data = is_data_url || substitute_data.IsValid() || archive_;
  if (is_static_data) {
    resource = ResourceForStaticData(request, factory, substitute_data);
    if (!resource && !is_data_url && archive_)
      return nullptr;
  }
  if (!resource) {
    resource =
        GetMemoryCache()->ResourceForURL(request.Url(), GetCacheIdentifier());
  }

  MakePreloadedResourceBlockOnloadIfNeeded(resource, request);

  const RevalidationPolicy policy = DetermineRevalidationPolicy(
      factory.GetType(), request, resource, is_static_data);
  TRACE_EVENT_INSTANT1("blink", "ResourceFetcher::determineRevalidationPolicy",
                       TRACE_EVENT_SCOPE_THREAD, "revalidationPolicy", policy);

  UpdateMemoryCacheStats(resource, policy, request, factory, is_static_data);

  switch (policy) {
    case kReload:
      GetMemoryCache()->Remove(resource);
    case kLoad:
      resource = CreateResourceForLoading(request, request.Charset(), factory);
      break;
    case kRevalidate:
      InitializeRevalidation(resource_request, resource);
      break;
    case kUse:
      if (resource->IsLinkPreload() && !request.IsLinkPreload())
        resource->SetLinkPreload(false);
      break;
  }
  if (!resource)
    return nullptr;

  CHECK_EQ(resource->GetType(), factory.GetType());

  if (!resource->IsAlive())
    dead_stats_recorder_.Update(policy);

  if (policy != kUse)
    resource->SetIdentifier(identifier);

  if (!request.IsSpeculativePreload() || policy != kUse) {
    if (resource_request.Priority() > resource->GetResourceRequest().Priority())
      resource->DidChangePriority(resource_request.Priority(), 0);
  }

  DCHECK(EqualIgnoringFragmentIdentifier(resource->Url(), request.Url()));
  RequestLoadStarted(identifier, resource, request, policy, is_static_data);
  document_resources_.Set(
      MemoryCache::RemoveFragmentIdentifierIfNeeded(request.Url()), resource);

  if (!ResourceNeedsLoad(resource, request, policy))
    return resource;

  if (!StartLoad(resource))
    return nullptr;
  scoped_resource_load_tracker.resourceLoadContinuesBeyondScope();

  DCHECK(!resource->ErrorOccurred() ||
         request.Options().synchronous_policy == kRequestSynchronously);
  return resource;
}
