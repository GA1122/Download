void ResourceFetcher::EmulateLoadStartedForInspector(
    Resource* resource,
    const KURL& url,
    WebURLRequest::RequestContext request_context,
    const AtomicString& initiator_name) {
  if (CachedResource(url))
    return;
  ResourceRequest resource_request(url);
  resource_request.SetRequestContext(request_context);
  FetchRequest request(resource_request, initiator_name, resource->Options());
  Context().CanRequest(resource->GetType(), resource->LastResourceRequest(),
                       resource->LastResourceRequest().Url(), request.Options(),
                       SecurityViolationReportingPolicy::kReport,
                       request.GetOriginRestriction());
  RequestLoadStarted(resource->Identifier(), resource, request, kUse);
}
