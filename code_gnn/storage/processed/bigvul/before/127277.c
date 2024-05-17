bool PageSerializer::shouldAddURL(const KURL& url)
{
    return url.isValid() && !m_resourceURLs.contains(url) && !url.protocolIsData();
}
