int RenderBox::instrinsicScrollbarLogicalWidth() const
{
    if (!hasOverflowClip())
        return 0;

    if (isHorizontalWritingMode() && style()->overflowY() == OSCROLL) {
        ASSERT(layer()->scrollableArea() && layer()->scrollableArea()->hasVerticalScrollbar());
        return verticalScrollbarWidth();
    }

    if (!isHorizontalWritingMode() && style()->overflowX() == OSCROLL) {
        ASSERT(layer()->scrollableArea() && layer()->scrollableArea()->hasHorizontalScrollbar());
        return horizontalScrollbarHeight();
    }

    return 0;
}
