void Document::activeChainNodeDetached(Element& element)
{
    if (!m_activeHoverElement)
        return;

    if (element != m_activeHoverElement)
        return;

    Node* activeNode = FlatTreeTraversal::parent(element);
    while (activeNode && activeNode->isElementNode() && !activeNode->layoutObject())
        activeNode = FlatTreeTraversal::parent(*activeNode);

    m_activeHoverElement = activeNode && activeNode->isElementNode() ? toElement(activeNode) : 0;
}
