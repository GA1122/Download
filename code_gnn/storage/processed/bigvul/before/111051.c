IntSize WebPagePrivate::absoluteVisibleOverflowSize() const
{
    if (!m_mainFrame->contentRenderer())
        return IntSize();

    return IntSize(m_mainFrame->contentRenderer()->rightAbsoluteVisibleOverflow(), m_mainFrame->contentRenderer()->bottomAbsoluteVisibleOverflow());
}
