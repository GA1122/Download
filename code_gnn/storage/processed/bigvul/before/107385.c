IntPoint Scrollbar::convertFromContainingView(const IntPoint& parentPoint) const
{
    if (m_scrollableArea)
        return m_scrollableArea->convertFromContainingViewToScrollbar(this, parentPoint);

    return Widget::convertFromContainingView(parentPoint);
}
