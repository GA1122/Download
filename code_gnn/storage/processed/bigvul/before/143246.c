void Document::registerNodeList(const LiveNodeListBase* list)
{
    DCHECK(!m_nodeLists[list->invalidationType()].contains(list));
    m_nodeLists[list->invalidationType()].add(list);
    if (list->isRootedAtTreeScope())
        m_listsInvalidatedAtDocument.add(list);
}
