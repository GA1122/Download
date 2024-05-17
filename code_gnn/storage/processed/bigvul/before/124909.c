LayoutUnit RenderBox::overrideContainingBlockContentLogicalHeight() const
{
    ASSERT(hasOverrideContainingBlockLogicalHeight());
    return gOverrideContainingBlockLogicalHeightMap->get(this);
}
