LayoutUnit RenderBox::containingBlockLogicalWidthForContent() const
{
    if (hasOverrideContainingBlockLogicalWidth())
        return overrideContainingBlockContentLogicalWidth();

    RenderBlock* cb = containingBlock();
    return cb->availableLogicalWidth();
}
