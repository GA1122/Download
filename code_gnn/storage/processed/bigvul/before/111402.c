static inline ResourceRequestCachePolicy toWebCoreCachePolicy(Platform::NetworkRequest::CachePolicy policy)
{
    switch (policy) {
    case Platform::NetworkRequest::UseProtocolCachePolicy:
        return UseProtocolCachePolicy;
    case Platform::NetworkRequest::ReloadIgnoringCacheData:
        return ReloadIgnoringCacheData;
    case Platform::NetworkRequest::ReturnCacheDataElseLoad:
        return ReturnCacheDataElseLoad;
    case Platform::NetworkRequest::ReturnCacheDataDontLoad:
        return ReturnCacheDataDontLoad;
    default:
        ASSERT_NOT_REACHED();
        return UseProtocolCachePolicy;
    }
}
