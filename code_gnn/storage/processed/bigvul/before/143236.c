PageVisibilityState Document::pageVisibilityState() const
{
    if (!m_frame || !m_frame->page())
        return PageVisibilityStateHidden;
    if (m_loadEventProgress >= UnloadVisibilityChangeInProgress)
        return PageVisibilityStateHidden;
    return m_frame->page()->visibilityState();
}
