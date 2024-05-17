void Document::hoveredNodeDetached(Element& element)
{
    if (!m_hoverNode)
        return;

    m_hoverNode->updateDistribution();
    if (element != m_hoverNode && (!m_hoverNode->isTextNode() || element != ComposedTreeTraversal::parent(*m_hoverNode)))
        return;

    m_hoverNode = ComposedTreeTraversal::parent(element);
    while (m_hoverNode && !m_hoverNode->layoutObject())
        m_hoverNode = ComposedTreeTraversal::parent(*m_hoverNode);

    if (!page()->isCursorVisible())
        return;

    if (frame())
        frame()->eventHandler().scheduleHoverStateUpdate();
}
