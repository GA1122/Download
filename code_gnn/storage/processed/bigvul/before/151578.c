Resource* ResourceFetcher::RequestResource(
    FetchParameters& params,
    const ResourceFactory& factory,
    const SubstituteData& substitute_data) {
  unsigned long identifier = CreateUniqueIdentifier();
  ResourceRequest& resource_request = params.MutableResourceRequest();
  network_instrumentation::ScopedResourceLoadTracker
      scoped_resource_load_tracker(identifier, resource_request);
  SCOPED_BLINK_UMA_HISTOGRAM_TIMER_THREAD_SAFE(
      "Blink.Fetch.RequestResourceTime");
  TRACE_EVENT1("blink", "ResourceFetcher::requestResource", "url",
               UrlForTraceEvent(params.Url()));

  ResourceRequestBlockedReason blocked_reason =
      ResourceRequestBlockedReason::kNone;

  PrepareRequestResult result = PrepareRequest(params, factory, substitute_data,
                                               identifier, blocked_reason);
  if (result == kAbort)
    return nullptr;
  if (result == kBlock)
    return ResourceForBlockedRequest(params, factory, blocked_reason);

  Resource::Type resource_type = factory.GetType();

  if (!params.IsSpeculativePreload()) {
    Context().RecordLoadingActivity(identifier, resource_request, resource_type,
                                    params.Options().initiator_info.name);
  }

  Resource* resource = nullptr;
  RevalidationPolicy policy = kLoad;

  bool is_data_url = resource_request.Url().ProtocolIsData();
  bool is_static_data = is_data_url || substitute_data.IsValid() || archive_;
  if (is_static_data) {
    resource = ResourceForStaticData(params, factory, substitute_data);
    if (resource) {
      policy =
          DetermineRevalidationPolicy(resource_type, params, *resource, true);
    } else if (!is_data_url && archive_) {
      return nullptr;
    }
  }

  if (!resource) {
    resource = MatchPreload(params, resource_type);
    if (resource) {
      policy = kUse;
      MakePreloadedResourceBlockOnloadIfNeeded(resource, params);
    } else if (IsMainThread()) {
      resource =
          GetMemoryCache()->ResourceForURL(params.Url(), GetCacheIdentifier());
      if (resource) {
        policy = DetermineRevalidationPolicy(resource_type, params, *resource,
                                             is_static_data);
      }
    }
  }

  UpdateMemoryCacheStats(resource, policy, params, factory, is_static_data);

  switch (policy) {
    case kReload:
      GetMemoryCache()->Remove(resource);
    case kLoad:
      resource = CreateResourceForLoading(params, factory);
      break;
    case kRevalidate:
      InitializeRevalidation(resource_request, resource);
      break;
    case kUse:
      if (resource->IsLinkPreload() && !params.IsLinkPreload())
        resource->SetLinkPreload(false);
      break;
  }
  DCHECK(resource);
  CHECK_EQ(resource->GetType(), resource_type);

  if (policy != kUse)
    resource->SetIdentifier(identifier);

  if (!params.IsSpeculativePreload() || policy != kUse) {
    if (resource_request.Priority() > resource->GetResourceRequest().Priority())
      resource->DidChangePriority(resource_request.Priority(), 0);
  }

  DCHECK(EqualIgnoringFragmentIdentifier(resource->Url(), params.Url()));
  RequestLoadStarted(identifier, resource, params, policy, is_static_data);
  cached_resources_map_.Set(
      MemoryCache::RemoveFragmentIdentifierIfNeeded(params.Url()), resource);
  document_resources_.insert(resource);

  if (!ResourceNeedsLoad(resource, params, policy)) {
    if (policy != kUse)
      InsertAsPreloadIfNecessary(resource, params, resource_type);
    return resource;
  }

  if (!StartLoad(resource))
    return nullptr;

  if (policy != kUse)
    InsertAsPreloadIfNecessary(resource, params, resource_type);
  scoped_resource_load_tracker.ResourceLoadContinuesBeyondScope();

  DCHECK(!resource->ErrorOccurred() ||
         params.Options().synchronous_policy == kRequestSynchronously);
  return resource;
}
