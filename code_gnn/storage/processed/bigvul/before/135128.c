void Document::activeChainNodeDetached(Element& element)
{
    if (!m_activeHoverElement)
        return;

    if (element != m_activeHoverElement)
        return;

    Node* activeNode = ComposedTreeTraversal::parent(element);
    while (activeNode && activeNode->isElementNode() && !activeNode->layoutObject())
        activeNode = ComposedTreeTraversal::parent(*activeNode);

    m_activeHoverElement = activeNode && activeNode->isElementNode() ? toElement(activeNode) : 0;
}
