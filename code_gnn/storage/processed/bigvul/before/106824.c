int RenderBox::baselinePosition(FontBaseline baselineType, bool  , LineDirectionMode direction, LinePositionMode  ) const
{
    if (isReplaced()) {
        int result = direction == HorizontalLine ? m_marginTop + height() + m_marginBottom : m_marginRight + width() + m_marginLeft;
        if (baselineType == AlphabeticBaseline)
            return result;
        return result - result / 2;
    }
    return 0;
}
