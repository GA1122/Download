void WebPagePrivate::loadString(const BlackBerry::Platform::String& string, const BlackBerry::Platform::String& baseURL, const BlackBerry::Platform::String& contentType, const BlackBerry::Platform::String& failingURL)
{
    KURL kurl = parseUrl(baseURL);
    ResourceRequest request(kurl);
    WTF::RefPtr<SharedBuffer> buffer
        = SharedBuffer::create(string.c_str(), string.length());
    SubstituteData substituteData(buffer,
        extractMIMETypeFromMediaType(contentType),
        extractCharsetFromMediaType(contentType),
        !failingURL.empty() ? parseUrl(failingURL) : KURL());
    m_mainFrame->loader()->load(FrameLoadRequest(m_mainFrame, request, substituteData));
}
