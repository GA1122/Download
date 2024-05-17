void MainResourceLoader::didReceiveData(const char* data, int length, long long lengthReceived, bool allAtOnce)
{
    ASSERT(data);
    ASSERT(length != 0);

    ASSERT(!m_response.isNull());

#if USE(CFNETWORK) || (PLATFORM(MAC) && !defined(BUILDING_ON_TIGER))
    if (m_response.isNull()) {
        m_response = ResourceResponse(KURL(), "text/html", 0, String(), String());
        if (DocumentLoader* documentLoader = frameLoader()->activeDocumentLoader())
            documentLoader->setResponse(m_response);
    }
#endif

#if !PLATFORM(CF)
    ASSERT(!defersLoading());
#endif
 
 #if ENABLE(OFFLINE_WEB_APPLICATIONS)
    documentLoader()->applicationCacheHost()->mainResourceDataReceived(data, length, lengthReceived, allAtOnce);
#endif

    RefPtr<MainResourceLoader> protect(this);

    m_timeOfLastDataReceived = currentTime();

    ResourceLoader::didReceiveData(data, length, lengthReceived, allAtOnce);
}
