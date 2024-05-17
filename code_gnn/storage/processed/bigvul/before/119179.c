bool XMLHttpRequest::isAllowedHTTPHeader(const String& name)
{
    initializeXMLHttpRequestStaticData();
    return !staticData->m_forbiddenRequestHeaders.contains(name) && !name.startsWith(staticData->m_proxyHeaderPrefix, false)
        && !name.startsWith(staticData->m_secHeaderPrefix, false);
}
