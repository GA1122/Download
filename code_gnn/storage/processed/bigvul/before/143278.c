bool Document::shouldInvalidateNodeListCaches(const QualifiedName* attrName) const
{
    if (attrName) {
        return shouldInvalidateNodeListCachesForAttr<DoNotInvalidateOnAttributeChanges + 1>(m_nodeLists, *attrName);
    }

    for (int type = 0; type < numNodeListInvalidationTypes; ++type) {
        if (!m_nodeLists[type].isEmpty())
            return true;
    }

    return false;
}
