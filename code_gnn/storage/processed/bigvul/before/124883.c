int RenderBox::horizontalScrollbarHeight() const
{
    if (!hasOverflowClip() || style()->overflowX() == OOVERLAY)
        return 0;

    return layer()->scrollableArea()->horizontalScrollbarHeight();
}
