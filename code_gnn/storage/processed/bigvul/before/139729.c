void ResourceFetcher::InitializeResourceRequest(
    ResourceRequest& request,
    Resource::Type type,
    FetchRequest::DeferOption defer) {
  if (request.GetCachePolicy() == WebCachePolicy::kUseProtocolCachePolicy) {
    request.SetCachePolicy(
        Context().ResourceRequestCachePolicy(request, type, defer));
  }
  if (request.GetRequestContext() == WebURLRequest::kRequestContextUnspecified)
    request.SetRequestContext(DetermineRequestContext(type));
  if (type == Resource::kLinkPrefetch)
    request.SetHTTPHeaderField(HTTPNames::Purpose, "prefetch");

  Context().AddAdditionalRequestHeaders(
      request, (type == Resource::kMainResource) ? kFetchMainResource
                                                 : kFetchSubresource);
}
