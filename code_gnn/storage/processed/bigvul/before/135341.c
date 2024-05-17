void Document::registerNodeListWithIdNameCache(const LiveNodeListBase* list)
{
#if ENABLE(OILPAN)
    m_nodeLists[InvalidateOnIdNameAttrChange].add(list);
#else
    m_nodeListCounts[InvalidateOnIdNameAttrChange]++;
#endif
}
