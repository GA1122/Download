void ResourceFetcher::EmulateLoadStartedForInspector(
    Resource* resource,
    const KURL& url,
    WebURLRequest::RequestContext request_context,
    const AtomicString& initiator_name) {
  if (CachedResource(url))
    return;
  ResourceRequest resource_request(url);
  resource_request.SetRequestContext(request_context);
  ResourceLoaderOptions options = resource->Options();
  options.initiator_info.name = initiator_name;
  FetchParameters params(resource_request, options);
  Context().CanRequest(resource->GetType(), resource->LastResourceRequest(),
                       resource->LastResourceRequest().Url(), params.Options(),
                       SecurityViolationReportingPolicy::kReport,
                       params.GetOriginRestriction(),
                       resource->LastResourceRequest().GetRedirectStatus());
  RequestLoadStarted(resource->Identifier(), resource, params, kUse);
}
