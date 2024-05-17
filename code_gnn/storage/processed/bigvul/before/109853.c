void Document::unregisterNodeList(LiveNodeListBase* list)
{
    if (list->hasIdNameCache())
        m_nodeListCounts[InvalidateOnIdNameAttrChange]--;
    m_nodeListCounts[list->invalidationType()]--;
    if (list->isRootedAtDocument()) {
        ASSERT(m_listsInvalidatedAtDocument.contains(list));
        m_listsInvalidatedAtDocument.remove(list);
    }
}
