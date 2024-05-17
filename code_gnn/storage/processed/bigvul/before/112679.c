void DocumentLoader::notifyFinished(CachedResource* resource)
{
    ASSERT_UNUSED(resource, m_mainResource == resource);
    ASSERT(m_mainResource);
    if (!m_mainResource->errorOccurred() && !m_mainResource->wasCanceled()) {
        finishedLoading(m_mainResource->loadFinishTime());
        return;
    }

#if !PLATFORM(CHROMIUM)
    if (m_request.cachePolicy() == ReturnCacheDataDontLoad && !m_mainResource->wasCanceled()) {
        frameLoader()->retryAfterFailedCacheOnlyMainResourceLoad();
        return;
    }
#endif

    mainReceivedError(m_mainResource->resourceError());
}
