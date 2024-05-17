LayoutUnit RenderBox::lineHeight(bool  , LineDirectionMode direction, LinePositionMode  ) const
{
    if (isReplaced())
        return direction == HorizontalLine ? m_marginBox.top() + height() + m_marginBox.bottom() : m_marginBox.right() + width() + m_marginBox.left();
    return 0;
}
