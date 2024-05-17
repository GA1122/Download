void Document::removeFocusedElementOfSubtree(Node* node, bool amongChildrenOnly)
{
    if (!m_focusedElement)
        return;

    if (!node->inDocument())
        return;
    bool contains = node->containsIncludingShadowDOM(m_focusedElement.get());
    if (contains && (m_focusedElement != node || !amongChildrenOnly))
        setFocusedElement(nullptr);
}
