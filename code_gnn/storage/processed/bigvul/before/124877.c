bool RenderBox::hasOverrideContainingBlockLogicalHeight() const
{
    return gOverrideContainingBlockLogicalHeightMap && gOverrideContainingBlockLogicalHeightMap->contains(this);
}
