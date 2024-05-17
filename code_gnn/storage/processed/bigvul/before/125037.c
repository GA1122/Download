LayoutUnit RenderFlexibleBox::mainAxisContentExtent(LayoutUnit contentLogicalHeight)
{
    if (isColumnFlow()) {
        LogicalExtentComputedValues computedValues;
        LayoutUnit borderPaddingAndScrollbar = borderAndPaddingLogicalHeight() + scrollbarLogicalHeight();
        LayoutUnit borderBoxLogicalHeight = std::max(contentLogicalHeight, contentLogicalHeight + borderPaddingAndScrollbar);
        computeLogicalHeight(borderBoxLogicalHeight, logicalTop(), computedValues);
        if (computedValues.m_extent == LayoutUnit::max())
            return computedValues.m_extent;
        return std::max(LayoutUnit(0), computedValues.m_extent - borderPaddingAndScrollbar);
    }
    return contentLogicalWidth();
}
