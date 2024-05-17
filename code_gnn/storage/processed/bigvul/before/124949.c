void RenderBox::setOverrideContainingBlockContentLogicalHeight(LayoutUnit logicalHeight)
{
    if (!gOverrideContainingBlockLogicalHeightMap)
        gOverrideContainingBlockLogicalHeightMap = new OverrideSizeMap;
    gOverrideContainingBlockLogicalHeightMap->set(this, logicalHeight);
}
