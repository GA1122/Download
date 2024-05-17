void Document::removeFullScreenElementOfSubtree(Node* node, bool amongChildrenOnly)
{
    if (!m_fullScreenElement)
        return;
    
    bool elementInSubtree = false;
    if (amongChildrenOnly)
        elementInSubtree = m_fullScreenElement->isDescendantOf(node);
    else
        elementInSubtree = (m_fullScreenElement == node) || m_fullScreenElement->isDescendantOf(node);
    
    if (elementInSubtree)
        fullScreenElementRemoved();
}
