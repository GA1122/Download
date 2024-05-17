RenderRegion* RenderBlock::regionAtBlockOffset(LayoutUnit blockOffset) const
{
    RenderFlowThread* flowThread = flowThreadContainingBlock();
    if (!flowThread || !flowThread->hasValidRegionInfo())
        return 0;

    return flowThread->regionAtBlockOffset(offsetFromLogicalTopOfFirstPage() + blockOffset, true);
}
