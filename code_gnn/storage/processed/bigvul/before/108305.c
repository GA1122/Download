void MainResourceLoader::handleDataLoadNow(MainResourceLoaderTimer*)
{
    RefPtr<MainResourceLoader> protect(this);

    KURL url = m_substituteData.responseURL();
    if (url.isEmpty())
        url = m_initialRequest.url();

    m_initialRequest = ResourceRequest();
        
    ResourceResponse response(url, m_substituteData.mimeType(), m_substituteData.content()->size(), m_substituteData.textEncoding(), "");
    didReceiveResponse(response);
}
