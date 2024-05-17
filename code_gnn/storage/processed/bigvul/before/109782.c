void Document::removeFocusedElementOfSubtree(Node* node, bool amongChildrenOnly)
{
    if (!m_focusedElement)
        return;

    Element* focusedElement = node->treeScope().adjustedFocusedElement();
    if (!focusedElement)
        return;

    bool nodeInSubtree = false;
    if (amongChildrenOnly)
        nodeInSubtree = focusedElement->isDescendantOf(node);
    else
        nodeInSubtree = (focusedElement == node) || focusedElement->isDescendantOf(node);

    if (nodeInSubtree)
        setFocusedElement(0);
}
