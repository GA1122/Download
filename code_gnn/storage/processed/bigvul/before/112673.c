void DocumentLoader::mainReceivedError(const ResourceError& error)
{
    ASSERT(!error.isNull());
    if (m_applicationCacheHost->maybeLoadFallbackForMainError(request(), error))
        return;

    if (m_identifierForLoadWithoutResourceLoader) {
        ASSERT(!mainResourceLoader());
        frameLoader()->client()->dispatchDidFailLoading(this, m_identifierForLoadWithoutResourceLoader, error);
    }

#if !USE(CF)
    ASSERT(!mainResourceLoader() || !mainResourceLoader()->defersLoading());
#endif

    m_applicationCacheHost->failedLoadingMainResource();

    if (!frameLoader())
        return;
    setMainDocumentError(error);
    clearMainResourceLoader();
    frameLoader()->receivedMainResourceError(error);
}
