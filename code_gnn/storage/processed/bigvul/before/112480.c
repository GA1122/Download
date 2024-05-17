void Document::hoveredNodeDetached(Node* node)
{
    if (!m_hoverNode || (node != m_hoverNode && (!m_hoverNode->isTextNode() || node != m_hoverNode->parentNode())))
        return;

    m_hoverNode = node->parentNode();
    while (m_hoverNode && !m_hoverNode->renderer())
        m_hoverNode = m_hoverNode->parentNode();
    if (frame())
        frame()->eventHandler()->scheduleHoverStateUpdate();
}
