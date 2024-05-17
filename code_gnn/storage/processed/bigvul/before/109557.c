void Document::activeChainNodeDetached(Node* node)
{
    if (!m_activeElement)
        return;

    if (node != m_activeElement && (!m_activeElement->isTextNode() || node != NodeRenderingTraversal::parent(m_activeElement.get())))
        return;

    Node* activeNode = NodeRenderingTraversal::parent(node);
    while (activeNode && activeNode->isElementNode() && !activeNode->renderer())
        activeNode = NodeRenderingTraversal::parent(activeNode);

    m_activeElement = activeNode && activeNode->isElementNode() ? toElement(activeNode) : 0;
}
