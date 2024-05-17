void RenderBlock::updateMinimumPageHeight(LayoutUnit offset, LayoutUnit minHeight)
{
    if (RenderFlowThread* flowThread = flowThreadContainingBlock())
        flowThread->updateMinimumPageHeight(offsetFromLogicalTopOfFirstPage() + offset, minHeight);
    else if (ColumnInfo* colInfo = view()->layoutState()->columnInfo())
        colInfo->updateMinimumColumnHeight(minHeight);
}
