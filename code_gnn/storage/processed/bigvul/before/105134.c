void Range::nodeChildrenWillBeRemoved(ContainerNode* container)
{
    ASSERT(container);
    ASSERT(container->document() == m_ownerDocument);
    boundaryNodeChildrenWillBeRemoved(m_start, container);
    boundaryNodeChildrenWillBeRemoved(m_end, container);
}
