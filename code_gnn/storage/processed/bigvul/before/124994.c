LayoutUnit RenderFlexibleBox::computeChildMarginValue(Length margin)
{
    LayoutUnit availableSize = contentLogicalWidth();
    return minimumValueForLength(margin, availableSize);
}
