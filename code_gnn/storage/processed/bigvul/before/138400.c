void Document::activeChainNodeDetached(Node* node)
{
    if (!m_activeHoverElement)
        return;

    if (node != m_activeHoverElement)
        return;

    Node* activeNode = ComposedTreeTraversal::parent(*node);
    while (activeNode && activeNode->isElementNode() && !activeNode->layoutObject())
        activeNode = ComposedTreeTraversal::parent(*activeNode);

    m_activeHoverElement = activeNode && activeNode->isElementNode() ? toElement(activeNode) : 0;
}
