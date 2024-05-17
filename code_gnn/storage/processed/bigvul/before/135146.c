AXObjectCache* Document::axObjectCache() const
{
    Settings* settings = this->settings();
    if (!settings || !settings->accessibilityEnabled())
        return 0;

    Document& cacheOwner = this->axObjectCacheOwner();

    if (!cacheOwner.layoutView())
        return 0;

    ASSERT(&cacheOwner == this || !m_axObjectCache);
    if (!cacheOwner.m_axObjectCache)
        cacheOwner.m_axObjectCache = AXObjectCache::create(cacheOwner);
    return cacheOwner.m_axObjectCache.get();
}
