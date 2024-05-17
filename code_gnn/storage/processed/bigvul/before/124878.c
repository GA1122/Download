bool RenderBox::hasOverrideContainingBlockLogicalWidth() const
{
    return gOverrideContainingBlockLogicalWidthMap && gOverrideContainingBlockLogicalWidthMap->contains(this);
}
