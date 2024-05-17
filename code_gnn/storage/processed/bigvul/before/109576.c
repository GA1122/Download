AXObjectCache* Document::axObjectCache() const
{
    if (!AXObjectCache::accessibilityEnabled())
        return 0;

    Document* topDocument = this->topDocument();

    if (!topDocument->renderer())
        return 0;

    ASSERT(topDocument == this || !m_axObjectCache);
    if (!topDocument->m_axObjectCache)
        topDocument->m_axObjectCache = adoptPtr(new AXObjectCache(topDocument));
    return topDocument->m_axObjectCache.get();
}
