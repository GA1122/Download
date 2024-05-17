mojom::FetchCacheMode DetermineCacheMode(RequestMethod method,
                                         RequestType request_type,
                                         ResourceType resource_type,
                                         WebFrameLoadType load_type) {
  switch (load_type) {
    case WebFrameLoadType::kStandard:
    case WebFrameLoadType::kReplaceCurrentItem:
      return (request_type == RequestType::kIsConditional ||
              method == RequestMethod::kIsPost)
                 ? mojom::FetchCacheMode::kValidateCache
                 : mojom::FetchCacheMode::kDefault;
    case WebFrameLoadType::kBackForward:
      return method == RequestMethod::kIsPost
                 ? mojom::FetchCacheMode::kOnlyIfCached
                 : mojom::FetchCacheMode::kForceCache;
    case WebFrameLoadType::kReload:
      return resource_type == ResourceType::kIsMainResource
                 ? mojom::FetchCacheMode::kValidateCache
                 : mojom::FetchCacheMode::kDefault;
    case WebFrameLoadType::kReloadBypassingCache:
      return mojom::FetchCacheMode::kBypassCache;
  }
  NOTREACHED();
  return mojom::FetchCacheMode::kDefault;
}
