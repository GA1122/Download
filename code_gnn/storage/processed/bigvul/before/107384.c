IntRect Scrollbar::convertFromContainingView(const IntRect& parentRect) const
{
    if (m_scrollableArea)
        return m_scrollableArea->convertFromContainingViewToScrollbar(this, parentRect);

    return Widget::convertFromContainingView(parentRect);
}
