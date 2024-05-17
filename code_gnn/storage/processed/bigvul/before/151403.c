WebCachePolicy DetermineFrameWebCachePolicy(Frame* frame,
                                            ResourceType resource_type) {
  if (!frame)
    return WebCachePolicy::kUseProtocolCachePolicy;
  if (!frame->IsLocalFrame())
    return DetermineFrameWebCachePolicy(frame->Tree().Parent(), resource_type);

  if (resource_type == ResourceType::kIsNotMainResource &&
      ToLocalFrame(frame)->GetDocument()->LoadEventFinished()) {
    return WebCachePolicy::kUseProtocolCachePolicy;
  }

  FrameLoadType load_type =
      ToLocalFrame(frame)->Loader().GetDocumentLoader()->LoadType();
  if (load_type == kFrameLoadTypeReloadBypassingCache)
    return WebCachePolicy::kBypassingCache;

  WebCachePolicy parent_policy =
      DetermineFrameWebCachePolicy(frame->Tree().Parent(), resource_type);
  if (parent_policy != WebCachePolicy::kUseProtocolCachePolicy)
    return parent_policy;

  return DetermineWebCachePolicy(RequestMethod::kIsNotPost,
                                 RequestType::kIsNotConditional,
                                 ResourceType::kIsNotMainResource, load_type);
}
