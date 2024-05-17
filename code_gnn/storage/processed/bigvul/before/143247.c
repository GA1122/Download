void Document::registerNodeListWithIdNameCache(const LiveNodeListBase* list)
{
    DCHECK(!m_nodeLists[InvalidateOnIdNameAttrChange].contains(list));
    m_nodeLists[InvalidateOnIdNameAttrChange].add(list);
}
