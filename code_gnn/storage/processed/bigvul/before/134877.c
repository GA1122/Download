WebPageSerializerImpl::WebPageSerializerImpl(WebFrame* frame,
                                             WebPageSerializerClient* client,
                                             const WebVector<WebURL>& links,
                                             const WebVector<WebString>& localPaths,
                                             const WebString& localDirectoryName)
    : m_client(client)
    , m_localDirectoryName(localDirectoryName)
    , m_htmlEntities(false)
    , m_xmlEntities(true)
{
    ASSERT(frame);
    m_specifiedWebLocalFrameImpl = toWebLocalFrameImpl(frame);
    ASSERT(client);
    ASSERT(links.size() == localPaths.size());
    for (size_t i = 0; i < links.size(); i++) {
        KURL url = links[i];
        ASSERT(!m_localLinks.contains(url.string()));
        m_localLinks.set(url.string(), localPaths[i]);
    }

    ASSERT(m_dataBuffer.isEmpty());
}
