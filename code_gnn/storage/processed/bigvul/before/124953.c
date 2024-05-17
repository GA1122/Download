void RenderBox::setScrollLeft(int newLeft)
{
    DisableCompositingQueryAsserts disabler;

    if (hasOverflowClip())
        layer()->scrollableArea()->scrollToXOffset(newLeft, ScrollOffsetClamped);
}
