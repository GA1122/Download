int RenderBox::verticalScrollbarWidth() const
{
    if (!hasOverflowClip() || style()->overflowY() == OOVERLAY)
        return 0;

    return layer()->scrollableArea()->verticalScrollbarWidth();
}
