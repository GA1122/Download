mojom::FetchCacheMode FrameFetchContext::ResourceRequestCachePolicy(
    const ResourceRequest& request,
    Resource::Type type,
    FetchParameters::DeferOption defer) const {
  if (IsDetached())
    return mojom::FetchCacheMode::kDefault;

  DCHECK(GetFrame());
  if (type == Resource::kMainResource) {
    const auto cache_mode = DetermineCacheMode(
        request.HttpMethod() == HTTPNames::POST ? RequestMethod::kIsPost
                                                : RequestMethod::kIsNotPost,
        request.IsConditional() ? RequestType::kIsConditional
                                : RequestType::kIsNotConditional,
        ResourceType::kIsMainResource, MasterDocumentLoader()->LoadType());
    if (cache_mode != mojom::FetchCacheMode::kDefault)
      return cache_mode;
    return DetermineFrameCacheMode(GetFrame()->Tree().Parent(),
                                   ResourceType::kIsMainResource);
  }

  const auto cache_mode =
      DetermineFrameCacheMode(GetFrame(), ResourceType::kIsNotMainResource);

  if (cache_mode == mojom::FetchCacheMode::kDefault &&
      request.IsConditional()) {
    return mojom::FetchCacheMode::kValidateCache;
  }
  return cache_mode;
}
