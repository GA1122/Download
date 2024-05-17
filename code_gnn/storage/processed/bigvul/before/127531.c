void LayerWebKitThread::setBounds(const IntSize& size)
{
    if (m_bounds == size)
        return;

    bool firstResize = !m_bounds.width() && !m_bounds.height() && size.width() && size.height();

    m_bounds = size;

    boundsChanged();

    if (firstResize)
        setNeedsDisplay();
    else
        setNeedsCommit();
}
