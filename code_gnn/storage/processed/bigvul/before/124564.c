int RenderBlock::inlineBlockBaseline(LineDirectionMode direction) const
{
    if (!style()->isOverflowVisible()) {
        return direction == HorizontalLine ? height() + m_marginBox.bottom() : width() + m_marginBox.left();
    }

    return lastLineBoxBaseline(direction);
}
