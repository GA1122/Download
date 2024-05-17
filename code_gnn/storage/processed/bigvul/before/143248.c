void Document::removeFocusedElementOfSubtree(Node* node, bool amongChildrenOnly)
{
    if (!m_focusedElement)
        return;

    if (!node->inShadowIncludingDocument())
        return;
    bool contains = node->isShadowIncludingInclusiveAncestorOf(m_focusedElement.get());
    if (contains && (m_focusedElement != node || !amongChildrenOnly))
        clearFocusedElement();
}
