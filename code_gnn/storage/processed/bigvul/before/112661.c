void DocumentLoader::handleSubstituteDataLoadNow(DocumentLoaderTimer*)
{
    KURL url = m_substituteData.responseURL();
    if (url.isEmpty())
        url = m_request.url();
    ResourceResponse response(url, m_substituteData.mimeType(), m_substituteData.content()->size(), m_substituteData.textEncoding(), "");
    responseReceived(0, response);
}
