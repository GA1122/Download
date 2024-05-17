void DocumentLoader::getSubresources(Vector<PassRefPtr<ArchiveResource> >& subresources) const
{
    if (!isCommitted())
        return;

    const CachedResourceLoader::DocumentResourceMap& allResources = m_cachedResourceLoader->allCachedResources();
    CachedResourceLoader::DocumentResourceMap::const_iterator end = allResources.end();
    for (CachedResourceLoader::DocumentResourceMap::const_iterator it = allResources.begin(); it != end; ++it) {
        RefPtr<ArchiveResource> subresource = this->subresource(KURL(ParsedURLString, it->value->url()));
        if (subresource)
            subresources.append(subresource.release());
    }

    return;
}
