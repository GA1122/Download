WebCachePolicy DetermineWebCachePolicy(RequestMethod method,
                                       RequestType request_type,
                                       ResourceType resource_type,
                                       FrameLoadType load_type) {
  switch (load_type) {
    case kFrameLoadTypeStandard:
    case kFrameLoadTypeReplaceCurrentItem:
    case kFrameLoadTypeInitialInChildFrame:
      return (request_type == RequestType::kIsConditional ||
              method == RequestMethod::kIsPost)
                 ? WebCachePolicy::kValidatingCacheData
                 : WebCachePolicy::kUseProtocolCachePolicy;
    case kFrameLoadTypeBackForward:
    case kFrameLoadTypeInitialHistoryLoad:
      return method == RequestMethod::kIsPost
                 ? WebCachePolicy::kReturnCacheDataDontLoad
                 : WebCachePolicy::kReturnCacheDataElseLoad;
    case kFrameLoadTypeReload:
      return resource_type == ResourceType::kIsMainResource
                 ? WebCachePolicy::kValidatingCacheData
                 : WebCachePolicy::kUseProtocolCachePolicy;
    case kFrameLoadTypeReloadBypassingCache:
      return WebCachePolicy::kBypassingCache;
  }
  NOTREACHED();
  return WebCachePolicy::kUseProtocolCachePolicy;
}
