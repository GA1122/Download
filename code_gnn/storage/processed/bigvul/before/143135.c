void Document::clearAXObjectCache()
{
    DCHECK_EQ(&axObjectCacheOwner(), this);
    if (m_axObjectCache)
        m_axObjectCache->dispose();
    m_axObjectCache.clear();
}
