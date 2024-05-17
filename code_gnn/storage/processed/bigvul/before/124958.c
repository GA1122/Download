void RenderBox::shrinkToFitWidth(const LayoutUnit availableSpace, const LayoutUnit logicalLeftValue, const LayoutUnit bordersPlusPadding, LogicalExtentComputedValues& computedValues) const
{
    LayoutUnit preferredWidth = maxPreferredLogicalWidth() - bordersPlusPadding;
    LayoutUnit preferredMinWidth = minPreferredLogicalWidth() - bordersPlusPadding;
    LayoutUnit availableWidth = availableSpace - logicalLeftValue;
    computedValues.m_extent = min(max(preferredMinWidth, availableWidth), preferredWidth);
}
