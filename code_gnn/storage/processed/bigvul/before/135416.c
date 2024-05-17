void Document::unregisterNodeListWithIdNameCache(const LiveNodeListBase* list)
{
#if ENABLE(OILPAN)
    ASSERT(m_nodeLists[InvalidateOnIdNameAttrChange].contains(list));
    m_nodeLists[InvalidateOnIdNameAttrChange].remove(list);
#else
    ASSERT(m_nodeListCounts[InvalidateOnIdNameAttrChange] > 0);
    m_nodeListCounts[InvalidateOnIdNameAttrChange]--;
#endif
}
