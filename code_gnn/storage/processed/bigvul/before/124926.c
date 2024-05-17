LayoutUnit RenderBox::perpendicularContainingBlockLogicalHeight() const
{
    if (hasOverrideContainingBlockLogicalHeight())
        return overrideContainingBlockContentLogicalHeight();

    RenderBlock* cb = containingBlock();
    if (cb->hasOverrideHeight())
        return cb->overrideLogicalContentHeight();

    RenderStyle* containingBlockStyle = cb->style();
    Length logicalHeightLength = containingBlockStyle->logicalHeight();

    if (!logicalHeightLength.isFixed()) {
        LayoutUnit fillFallbackExtent = containingBlockStyle->isHorizontalWritingMode() ? view()->frameView()->visibleHeight() : view()->frameView()->visibleWidth();
        LayoutUnit fillAvailableExtent = containingBlock()->availableLogicalHeight(ExcludeMarginBorderPadding);
        return min(fillAvailableExtent, fillFallbackExtent);
    }

    return cb->adjustContentBoxLogicalHeightForBoxSizing(logicalHeightLength.value());
}
