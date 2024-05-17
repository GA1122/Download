void Document::invalidateNodeListCaches(const QualifiedName* attrName)
{
    for (const LiveNodeListBase* list : m_listsInvalidatedAtDocument)
        list->invalidateCacheForAttribute(attrName);
}
