KURL Document::completeURL(const String& url) const
{
    return completeURLWithOverride(url, m_baseURL);
}
