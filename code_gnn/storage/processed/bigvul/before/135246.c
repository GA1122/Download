AXObjectCache* Document::existingAXObjectCache() const
{
    if (!axObjectCacheOwner().layoutView())
        return 0;

    return axObjectCacheOwner().m_axObjectCache.get();
}
