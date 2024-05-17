bool Document::shouldInvalidateNodeListCaches(const QualifiedName* attrName) const
{
    if (attrName) {
#if ENABLE(OILPAN)
        return shouldInvalidateNodeListCachesForAttr<DoNotInvalidateOnAttributeChanges + 1>(m_nodeLists, *attrName);
#else
        return shouldInvalidateNodeListCachesForAttr<DoNotInvalidateOnAttributeChanges + 1>(m_nodeListCounts, *attrName);
#endif
    }

    for (int type = 0; type < numNodeListInvalidationTypes; ++type) {
#if ENABLE(OILPAN)
        if (!m_nodeLists[type].isEmpty())
#else
        if (m_nodeListCounts[type])
#endif
            return true;
    }

    return false;
}
