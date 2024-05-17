bool XSSAuditor::isLikelySafeResource(const String& url)
{
    if (url.isEmpty() || url == blankURL().string())
        return true;

    if (m_documentURL.host().isEmpty())
        return false;

    KURL resourceURL(m_documentURL, url);
    return (m_documentURL.host() == resourceURL.host() && resourceURL.query().isEmpty());
}
