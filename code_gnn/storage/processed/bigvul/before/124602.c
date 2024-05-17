LayoutUnit RenderBlock::pageLogicalHeightForOffset(LayoutUnit offset) const
{
    RenderView* renderView = view();
    RenderFlowThread* flowThread = flowThreadContainingBlock();
    if (!flowThread)
        return renderView->layoutState()->pageLogicalHeight();
    return flowThread->pageLogicalHeightForOffset(offset + offsetFromLogicalTopOfFirstPage());
}
