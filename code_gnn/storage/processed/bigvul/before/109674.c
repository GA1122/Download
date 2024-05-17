AXObjectCache* Document::existingAXObjectCache() const
{
    if (!AXObjectCache::accessibilityEnabled())
        return 0;

    if (!topDocument()->renderer())
        return 0;

    return topDocument()->m_axObjectCache.get();
}
