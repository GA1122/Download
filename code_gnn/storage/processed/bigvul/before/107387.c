IntPoint Scrollbar::convertToContainingView(const IntPoint& localPoint) const
{
    if (m_scrollableArea)
        return m_scrollableArea->convertFromScrollbarToContainingView(this, localPoint);

    return Widget::convertToContainingView(localPoint);
}
