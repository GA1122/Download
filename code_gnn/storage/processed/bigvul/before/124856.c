LayoutUnit RenderBox::containingBlockLogicalHeightForContent(AvailableLogicalHeightType heightType) const
{
    if (hasOverrideContainingBlockLogicalHeight())
        return overrideContainingBlockContentLogicalHeight();

    RenderBlock* cb = containingBlock();
    return cb->availableLogicalHeight(heightType);
}
