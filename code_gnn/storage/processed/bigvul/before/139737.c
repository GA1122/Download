ResourceFetcher::PrepareRequestResult ResourceFetcher::PrepareRequest(
    FetchRequest& request,
    const ResourceFactory& factory,
    const SubstituteData& substitute_data,
    unsigned long identifier,
    ResourceRequestBlockedReason& blocked_reason) {
  ResourceRequest& resource_request = request.MutableResourceRequest();

  DCHECK(request.Options().synchronous_policy == kRequestAsynchronously ||
         factory.GetType() == Resource::kRaw ||
         factory.GetType() == Resource::kXSLStyleSheet);

  Context().PopulateResourceRequest(
      factory.GetType(), request.GetClientHintsPreferences(),
      request.GetResourceWidth(), resource_request);

  if (!request.Url().IsValid())
    return kAbort;

  resource_request.SetPriority(ComputeLoadPriority(
      factory.GetType(), request.GetResourceRequest(),
      ResourcePriority::kNotVisible, request.Defer(),
      request.IsSpeculativePreload(), request.IsLinkPreload()));
  InitializeResourceRequest(resource_request, factory.GetType(),
                            request.Defer());
  network_instrumentation::resourcePrioritySet(identifier,
                                               resource_request.Priority());

  blocked_reason = Context().CanRequest(
      factory.GetType(), resource_request,
      MemoryCache::RemoveFragmentIdentifierIfNeeded(request.Url()),
      request.Options(),
       
      request.IsSpeculativePreload()
          ? SecurityViolationReportingPolicy::kSuppressReporting
          : SecurityViolationReportingPolicy::kReport,
      request.GetOriginRestriction());
  if (blocked_reason != ResourceRequestBlockedReason::kNone) {
    DCHECK(!substitute_data.ForceSynchronousLoad());
    return kBlock;
  }

  Context().PrepareRequest(resource_request,
                           FetchContext::RedirectType::kNotForRedirect);

  if (!request.Url().IsValid())
    return kAbort;

  resource_request.SetAllowStoredCredentials(
      request.Options().allow_credentials == kAllowStoredCredentials);
  return kContinue;
}
