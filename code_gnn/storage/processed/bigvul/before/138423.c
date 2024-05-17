void Document::hoveredNodeDetached(Node* node)
{
    ASSERT(node);
    if (!m_hoverNode)
        return;

    m_hoverNode->updateDistribution();
    if (node != m_hoverNode && (!m_hoverNode->isTextNode() || node != ComposedTreeTraversal::parent(*m_hoverNode)))
        return;

    m_hoverNode = ComposedTreeTraversal::parent(*node);
    while (m_hoverNode && !m_hoverNode->layoutObject())
        m_hoverNode = ComposedTreeTraversal::parent(*m_hoverNode);

    if (!page()->isCursorVisible())
        return;

    if (frame())
        frame()->eventHandler().scheduleHoverStateUpdate();
}
