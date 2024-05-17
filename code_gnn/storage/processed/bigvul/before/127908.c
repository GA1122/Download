void DocumentThreadableLoader::loadFallbackRequestForServiceWorker()
{
    clearResource();
    ResourceRequest fallbackRequest(m_fallbackRequestForServiceWorker);
    m_fallbackRequestForServiceWorker = ResourceRequest();
    dispatchInitialRequest(fallbackRequest);
}
