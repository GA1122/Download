KURL DocumentLoader::documentURL() const
{
    KURL url = substituteData().responseURL();
#if ENABLE(WEB_ARCHIVE)
    if (url.isEmpty() && m_archive && m_archive->type() == Archive::WebArchive)
        url = m_archive->mainResource()->url();
#endif
    if (url.isEmpty())
        url = requestURL();
    if (url.isEmpty())
        url = m_response.url();
    return url;
}
