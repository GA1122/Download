IntRect Scrollbar::convertToContainingView(const IntRect& localRect) const
{
    if (m_scrollableArea)
        return m_scrollableArea->convertFromScrollbarToContainingView(this, localRect);

    return Widget::convertToContainingView(localRect);
}
