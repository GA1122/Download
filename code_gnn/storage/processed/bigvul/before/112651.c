void DocumentLoader::dataReceived(CachedResource* resource, const char* data, int length)
{
    ASSERT(data);
    ASSERT(length);
    ASSERT_UNUSED(resource, resource == m_mainResource);
    ASSERT(!m_response.isNull());

#if USE(CFNETWORK) || PLATFORM(MAC)
    if (m_response.isNull())
        m_response = ResourceResponse(KURL(), "text/html", 0, String(), String());
#endif

#if !USE(CF)
    ASSERT(!mainResourceLoader() || !mainResourceLoader()->defersLoading());
#endif

#if USE(CONTENT_FILTERING)
    bool loadWasBlockedBeforeFinishing = false;
    if (m_contentFilter && m_contentFilter->needsMoreData()) {
        m_contentFilter->addData(data, length);

        if (m_contentFilter->needsMoreData()) {
            commitLoad(0, 0);
            return;
        }

        data = m_contentFilter->getReplacementData(length);
        loadWasBlockedBeforeFinishing = m_contentFilter->didBlockData();
    }
#endif

    if (m_identifierForLoadWithoutResourceLoader)
        frameLoader()->notifier()->dispatchDidReceiveData(this, m_identifierForLoadWithoutResourceLoader, data, length, -1);

    m_applicationCacheHost->mainResourceDataReceived(data, length, -1, false);
    m_timeOfLastDataReceived = monotonicallyIncreasingTime();

    if (!isMultipartReplacingLoad())
        commitLoad(data, length);

#if USE(CONTENT_FILTERING)
    if (loadWasBlockedBeforeFinishing)
        cancelMainResourceLoad(frameLoader()->cancelledError(m_request));
#endif
}
