void RenderBox::clearContainingBlockOverrideSize()
{
    if (gOverrideContainingBlockLogicalWidthMap)
        gOverrideContainingBlockLogicalWidthMap->remove(this);
    clearOverrideContainingBlockContentLogicalHeight();
}
