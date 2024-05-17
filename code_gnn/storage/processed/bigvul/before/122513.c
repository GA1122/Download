void InspectorResourceAgent::willLoadXHR(XMLHttpRequest* xhr, ThreadableLoaderClient* client, const AtomicString& method, const KURL& url, bool async, FormData* formData, const HTTPHeaderMap& headers, bool includeCredentials)
{
    ASSERT(xhr);
    RefPtr<XHRReplayData> xhrReplayData = XHRReplayData::create(xhr->executionContext(), method, urlWithoutFragment(url), async, formData, includeCredentials);
    HTTPHeaderMap::const_iterator end = headers.end();
    for (HTTPHeaderMap::const_iterator it = headers.begin(); it!= end; ++it)
        xhrReplayData->addHeader(it->key, it->value);
    m_pendingXHRReplayData.set(client, xhrReplayData);
}
