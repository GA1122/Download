void Document::activeChainNodeDetached(Node* node)
{
    if (!m_activeElement || (node != m_activeElement && (!m_activeElement->isTextNode() || node != m_activeElement->parentNode())))
        return;

    m_activeElement = node->parentElement();
    while (m_activeElement && !m_activeElement->renderer())
        m_activeElement = m_activeElement->parentElement();
}
