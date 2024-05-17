WebCachePolicy FrameFetchContext::ResourceRequestCachePolicy(
    const ResourceRequest& request,
    Resource::Type type,
    FetchParameters::DeferOption defer) const {
  if (IsDetached())
    return WebCachePolicy::kUseProtocolCachePolicy;

  DCHECK(GetFrame());
  if (type == Resource::kMainResource) {
    const WebCachePolicy cache_policy = DetermineWebCachePolicy(
        request.HttpMethod() == "POST" ? RequestMethod::kIsPost
                                       : RequestMethod::kIsNotPost,
        request.IsConditional() ? RequestType::kIsConditional
                                : RequestType::kIsNotConditional,
        ResourceType::kIsMainResource, MasterDocumentLoader()->LoadType());
    if (cache_policy != WebCachePolicy::kUseProtocolCachePolicy)
      return cache_policy;
    return DetermineFrameWebCachePolicy(GetFrame()->Tree().Parent(),
                                        ResourceType::kIsMainResource);
  }

  const WebCachePolicy cache_policy = DetermineFrameWebCachePolicy(
      GetFrame(), ResourceType::kIsNotMainResource);

  if (cache_policy == WebCachePolicy::kUseProtocolCachePolicy &&
      request.IsConditional()) {
    return WebCachePolicy::kValidatingCacheData;
  }
  return cache_policy;
}
