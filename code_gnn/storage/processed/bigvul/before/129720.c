bool ResourceFetcher::isPreloaded(const String& urlString) const
{
    const KURL& url = m_document->completeURL(urlString);

    if (m_preloads) {
        ListHashSet<Resource*>::iterator end = m_preloads->end();
        for (ListHashSet<Resource*>::iterator it = m_preloads->begin(); it != end; ++it) {
            Resource* resource = *it;
            if (resource->url() == url)
                return true;
        }
    }

    return false;
}
