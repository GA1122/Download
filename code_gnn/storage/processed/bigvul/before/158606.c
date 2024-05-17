mojom::FetchCacheMode DetermineFrameCacheMode(Frame* frame,
                                              ResourceType resource_type) {
  if (!frame)
    return mojom::FetchCacheMode::kDefault;
  if (!frame->IsLocalFrame())
    return DetermineFrameCacheMode(frame->Tree().Parent(), resource_type);

  if (resource_type == ResourceType::kIsNotMainResource &&
      ToLocalFrame(frame)->GetDocument()->LoadEventFinished()) {
    return mojom::FetchCacheMode::kDefault;
  }

  WebFrameLoadType load_type =
      ToLocalFrame(frame)->Loader().GetDocumentLoader()->LoadType();
  if (load_type == WebFrameLoadType::kReloadBypassingCache)
    return mojom::FetchCacheMode::kBypassCache;

  mojom::FetchCacheMode parent_cache_mode =
      DetermineFrameCacheMode(frame->Tree().Parent(), resource_type);
  if (parent_cache_mode != mojom::FetchCacheMode::kDefault)
    return parent_cache_mode;

  return DetermineCacheMode(RequestMethod::kIsNotPost,
                            RequestType::kIsNotConditional,
                            ResourceType::kIsNotMainResource, load_type);
}
