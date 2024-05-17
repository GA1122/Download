void Scrollbar::invalidateRect(const IntRect& rect)
{
    if (suppressInvalidation())
        return;
    if (m_scrollableArea)
        m_scrollableArea->invalidateScrollbarRect(this, rect);
}
