void Document::unregisterNodeListWithIdNameCache(const LiveNodeListBase* list)
{
    DCHECK(m_nodeLists[InvalidateOnIdNameAttrChange].contains(list));
    m_nodeLists[InvalidateOnIdNameAttrChange].remove(list);
}
