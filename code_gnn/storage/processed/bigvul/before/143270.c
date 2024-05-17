void Document::setSequentialFocusNavigationStartingPoint(Node* node)
{
    if (!m_frame)
        return;
    if (!node) {
        m_sequentialFocusNavigationStartingPoint = nullptr;
        return;
    }
    DCHECK_EQ(node->document(), this);
    if (!m_sequentialFocusNavigationStartingPoint)
        m_sequentialFocusNavigationStartingPoint = Range::create(*this);
    m_sequentialFocusNavigationStartingPoint->selectNodeContents(node, ASSERT_NO_EXCEPTION);
}
