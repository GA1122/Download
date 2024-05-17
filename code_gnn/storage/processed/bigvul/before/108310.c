bool MainResourceLoader::load(const ResourceRequest& r, const SubstituteData& substituteData)
{
    ASSERT(!m_handle);

    m_substituteData = substituteData;

    ASSERT(documentLoader()->timing()->navigationStart);
    ASSERT(!documentLoader()->timing()->fetchStart);
    documentLoader()->timing()->fetchStart = currentTime();
    ResourceRequest request(r);

#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    documentLoader()->applicationCacheHost()->maybeLoadMainResource(request, m_substituteData);
#endif

    bool defer = defersLoading();
    if (defer) {
        bool shouldLoadEmpty = shouldLoadAsEmptyDocument(request.url());
        if (shouldLoadEmpty)
            defer = false;
    }
    if (!defer) {
        if (loadNow(request)) {
            ASSERT(defersLoading());
            defer = true;
        }
    }
    if (defer)
        m_initialRequest = request;

    return true;
}
