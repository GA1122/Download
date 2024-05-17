void Document::unregisterNodeList(const LiveNodeListBase* list)
{
    DCHECK(m_nodeLists[list->invalidationType()].contains(list));
    m_nodeLists[list->invalidationType()].remove(list);
    if (list->isRootedAtTreeScope()) {
        DCHECK(m_listsInvalidatedAtDocument.contains(list));
        m_listsInvalidatedAtDocument.remove(list);
    }
}
