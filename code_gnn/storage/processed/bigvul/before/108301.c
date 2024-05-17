void MainResourceLoader::didFail(const ResourceError& error)
{
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    if (documentLoader()->applicationCacheHost()->maybeLoadFallbackForMainError(request(), error))
        return;
#endif

#if !PLATFORM(CF)
    ASSERT(!defersLoading());
#endif
    
    receivedError(error);
}
