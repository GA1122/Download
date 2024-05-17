String WebPagePrivate::findPatternStringForUrl(const KURL& url) const
{
    if ((m_webSettings->shouldHandlePatternUrls() && protocolIs(url, "pattern"))
            || protocolIs(url, "tel")
            || protocolIs(url, "wtai")
            || protocolIs(url, "cti")
            || protocolIs(url, "mailto")
            || protocolIs(url, "sms")
            || protocolIs(url, "pin")) {
        return url;
    }
    return String();
}
