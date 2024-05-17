Element* Document::sequentialFocusNavigationStartingPoint(WebFocusType type) const
{
    if (m_focusedElement)
        return m_focusedElement.get();
    if (!m_sequentialFocusNavigationStartingPoint)
        return nullptr;
    if (!m_sequentialFocusNavigationStartingPoint->collapsed()) {
        Node* node = m_sequentialFocusNavigationStartingPoint->startContainer();
        DCHECK_EQ(node, m_sequentialFocusNavigationStartingPoint->endContainer());
        if (node->isElementNode())
            return toElement(node);
        if (Element* neighborElement = type == WebFocusTypeForward ? ElementTraversal::previous(*node) : ElementTraversal::next(*node))
            return neighborElement;
        return node->parentOrShadowHostElement();
    }

    if (m_sequentialFocusNavigationStartingPoint->startContainer()->isElementNode()
        && !m_sequentialFocusNavigationStartingPoint->startContainer()->hasChildren()
        && m_sequentialFocusNavigationStartingPoint->startOffset() == 0)
        return toElement(m_sequentialFocusNavigationStartingPoint->startContainer());

    if (Node* nextNode = m_sequentialFocusNavigationStartingPoint->firstNode()) {
        if (type == WebFocusTypeForward)
            return ElementTraversal::previous(*nextNode);
        if (nextNode->isElementNode())
            return toElement(nextNode);
        return ElementTraversal::next(*nextNode);
    }
    return nullptr;
}
