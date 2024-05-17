LayoutUnit RenderBox::computeIntrinsicLogicalContentHeightUsing(Length logicalHeightLength, LayoutUnit intrinsicContentHeight, LayoutUnit borderAndPadding) const
{
    if (logicalHeightLength.isMinContent() || logicalHeightLength.isMaxContent() || logicalHeightLength.isFitContent()) {
        if (isReplaced())
            return intrinsicSize().height();
        if (m_intrinsicContentLogicalHeight != -1)
            return m_intrinsicContentLogicalHeight;
        return intrinsicContentHeight;
    }
    if (logicalHeightLength.isFillAvailable())
        return containingBlock()->availableLogicalHeight(ExcludeMarginBorderPadding) - borderAndPadding;
    ASSERT_NOT_REACHED();
    return 0;
}
