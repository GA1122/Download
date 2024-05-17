void WebFrame::removeChild(WebFrame* child)
{
    child->m_parent = 0;

    if (m_firstChild == child)
        m_firstChild = child->m_nextSibling;
    else
        child->m_previousSibling->m_nextSibling = child->m_nextSibling;

    if (m_lastChild == child)
        m_lastChild = child->m_previousSibling;
    else
        child->m_nextSibling->m_previousSibling = child->m_previousSibling;

    child->m_previousSibling = child->m_nextSibling = 0;

    toImplBase()->frame()->tree().invalidateScopedChildCount();
    toImplBase()->frame()->host()->decrementSubframeCount();
}
