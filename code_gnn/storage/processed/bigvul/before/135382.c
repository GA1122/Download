void Document::setURL(const KURL& url)
{
    const KURL& newURL = url.isEmpty() ? blankURL() : url;
    if (newURL == m_url)
        return;

    m_url = newURL;
    m_accessEntryFromURL = nullptr;
    updateBaseURL();
    contextFeatures().urlDidChange(this);
}
