KURL Document::completeURL(const String& url, const KURL& baseURLOverride) const
{
    if (url.isNull())
        return KURL();
    const KURL& baseURL = ((baseURLOverride.isEmpty() || baseURLOverride == blankURL()) && parentDocument()) ? parentDocument()->baseURL() : baseURLOverride;
    if (!m_decoder)
        return KURL(baseURL, url);
    return KURL(baseURL, url, m_decoder->encoding());
}
