PageVisibilityState Document::pageVisibilityState() const
{
    if (!m_frame || !m_frame->page())
        return PageVisibilityStateHidden;
    return m_frame->page()->visibilityState();
}
