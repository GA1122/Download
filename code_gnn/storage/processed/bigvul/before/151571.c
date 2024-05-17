ResourceFetcher::PrepareRequestResult ResourceFetcher::PrepareRequest(
    FetchParameters& params,
    const ResourceFactory& factory,
    const SubstituteData& substitute_data,
    unsigned long identifier,
    ResourceRequestBlockedReason& blocked_reason) {
  ResourceRequest& resource_request = params.MutableResourceRequest();
  Resource::Type resource_type = factory.GetType();
  const ResourceLoaderOptions& options = params.Options();

  DCHECK(options.synchronous_policy == kRequestAsynchronously ||
         resource_type == Resource::kRaw ||
         resource_type == Resource::kXSLStyleSheet);

  params.OverrideContentType(factory.ContentType());

  SecurityViolationReportingPolicy reporting_policy =
      params.IsSpeculativePreload()
          ? SecurityViolationReportingPolicy::kSuppressReporting
          : SecurityViolationReportingPolicy::kReport;


  Context().CheckCSPForRequest(
      resource_request.GetRequestContext(),
      MemoryCache::RemoveFragmentIdentifierIfNeeded(params.Url()), options,
      reporting_policy, resource_request.GetRedirectStatus());

  Context().PopulateResourceRequest(
      resource_type, params.GetClientHintsPreferences(),
      params.GetResourceWidth(), resource_request);

  if (!params.Url().IsValid())
    return kAbort;

  resource_request.SetPriority(ComputeLoadPriority(
      resource_type, params.GetResourceRequest(), ResourcePriority::kNotVisible,
      params.Defer(), params.GetSpeculativePreloadType(),
      params.IsLinkPreload()));
  if (resource_request.GetCachePolicy() ==
      WebCachePolicy::kUseProtocolCachePolicy) {
    resource_request.SetCachePolicy(Context().ResourceRequestCachePolicy(
        resource_request, resource_type, params.Defer()));
  }
  if (resource_request.GetRequestContext() ==
      WebURLRequest::kRequestContextUnspecified) {
    resource_request.SetRequestContext(DetermineRequestContext(
        resource_type, kImageNotImageSet, Context().IsMainFrame()));
  }
  if (resource_type == Resource::kLinkPrefetch)
    resource_request.SetHTTPHeaderField(HTTPNames::Purpose, "prefetch");

  Context().AddAdditionalRequestHeaders(
      resource_request, (resource_type == Resource::kMainResource)
                            ? kFetchMainResource
                            : kFetchSubresource);

  network_instrumentation::ResourcePrioritySet(identifier,
                                               resource_request.Priority());

  blocked_reason = Context().CanRequest(
      resource_type, resource_request,
      MemoryCache::RemoveFragmentIdentifierIfNeeded(params.Url()), options,
      reporting_policy, params.GetOriginRestriction(),
      resource_request.GetRedirectStatus());
  if (blocked_reason != ResourceRequestBlockedReason::kNone)
    return kBlock;

  Context().PrepareRequest(resource_request,
                           FetchContext::RedirectType::kNotForRedirect);

  if (!params.Url().IsValid())
    return kAbort;

  RefPtr<SecurityOrigin> origin = options.security_origin;
  params.MutableOptions().cors_flag =
      !origin || !origin->CanRequestNoSuborigin(params.Url());

  if (options.cors_handling_by_resource_fetcher ==
      kEnableCORSHandlingByResourceFetcher) {
    bool allow_stored_credentials = false;
    switch (resource_request.GetFetchCredentialsMode()) {
      case WebURLRequest::kFetchCredentialsModeOmit:
        break;
      case WebURLRequest::kFetchCredentialsModeSameOrigin:
        allow_stored_credentials =
            !params.Options().cors_flag ||
            (origin &&
             origin->HasSuboriginAndShouldAllowCredentialsFor(params.Url()));
        break;
      case WebURLRequest::kFetchCredentialsModeInclude:
      case WebURLRequest::kFetchCredentialsModePassword:
        allow_stored_credentials = true;
        break;
    }
    resource_request.SetAllowStoredCredentials(allow_stored_credentials);
  }

  return kContinue;
}
