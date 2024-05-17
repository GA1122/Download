void RenderBox::setScrollTop(int newTop)
{
    DisableCompositingQueryAsserts disabler;

    if (hasOverflowClip())
        layer()->scrollableArea()->scrollToYOffset(newTop, ScrollOffsetClamped);
}
