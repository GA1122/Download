int RenderBox::scrollLeft() const
{
    return hasOverflowClip() ? layer()->scrollableArea()->scrollXOffset() : 0;
}
