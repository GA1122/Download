void Document::unregisterNodeList(const LiveNodeListBase* list)
{
#if ENABLE(OILPAN)
    ASSERT(m_nodeLists[list->invalidationType()].contains(list));
    m_nodeLists[list->invalidationType()].remove(list);
#else
    m_nodeListCounts[list->invalidationType()]--;
#endif
    if (list->isRootedAtTreeScope()) {
        ASSERT(m_listsInvalidatedAtDocument.contains(list));
        m_listsInvalidatedAtDocument.remove(list);
    }
}
