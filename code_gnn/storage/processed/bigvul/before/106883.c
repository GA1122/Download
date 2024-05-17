int RenderBox::lineHeight(bool  , LineDirectionMode direction, LinePositionMode  ) const
{
    if (isReplaced())
        return direction == HorizontalLine ? m_marginTop + height() + m_marginBottom : m_marginRight + width() + m_marginLeft;
    return 0;
}
