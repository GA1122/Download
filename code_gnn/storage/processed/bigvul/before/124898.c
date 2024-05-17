void RenderBox::markForPaginationRelayoutIfNeeded(SubtreeLayoutScope& layoutScope)
{
    ASSERT(!needsLayout());
    if (view()->layoutState()->pageLogicalHeightChanged() && firstChild())
        layoutScope.setChildNeedsLayout(this);
}
