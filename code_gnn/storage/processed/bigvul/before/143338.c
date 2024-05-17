void WebFrame::insertAfter(WebFrame* newChild, WebFrame* previousSibling)
{
    newChild->m_parent = this;

    WebFrame* next;
    if (!previousSibling) {
        next = m_firstChild;
        m_firstChild = newChild;
    } else {
        DCHECK_EQ(previousSibling->m_parent, this);
        next = previousSibling->m_nextSibling;
        previousSibling->m_nextSibling = newChild;
        newChild->m_previousSibling = previousSibling;
    }

    if (next) {
        newChild->m_nextSibling = next;
        next->m_previousSibling = newChild;
    } else {
        m_lastChild = newChild;
    }

    toImplBase()->frame()->tree().invalidateScopedChildCount();
    toImplBase()->frame()->host()->incrementSubframeCount();
}
