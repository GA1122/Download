void RenderBox::scrollToOffset(const IntSize& offset)
{
    ASSERT(hasOverflowClip());

    DisableCompositingQueryAsserts disabler;
    layer()->scrollableArea()->scrollToOffset(offset, ScrollOffsetClamped);
}
