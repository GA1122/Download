void RenderBox::clearOverrideContainingBlockContentLogicalHeight()
{
    if (gOverrideContainingBlockLogicalHeightMap)
        gOverrideContainingBlockLogicalHeightMap->remove(this);
}
