KURL FrameLoader::iconURL()
{
    if (m_frame->tree() && m_frame->tree()->parent())
        return KURL();

    if (!m_frame->document()->iconURL().isEmpty())
        return KURL(ParsedURLString, m_frame->document()->iconURL());

    if (!m_URL.protocolInHTTPFamily())
        return KURL();

    KURL url;
    bool couldSetProtocol = url.setProtocol(m_URL.protocol());
    ASSERT_UNUSED(couldSetProtocol, couldSetProtocol);
    url.setHost(m_URL.host());
    if (m_URL.hasPort())
        url.setPort(m_URL.port());
    url.setPath("/favicon.ico");
    return url;
}
