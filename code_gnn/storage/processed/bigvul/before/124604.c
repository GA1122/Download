LayoutUnit RenderBlock::pageRemainingLogicalHeightForOffset(LayoutUnit offset, PageBoundaryRule pageBoundaryRule) const
{
    RenderView* renderView = view();
    offset += offsetFromLogicalTopOfFirstPage();

    RenderFlowThread* flowThread = flowThreadContainingBlock();
    if (!flowThread) {
        LayoutUnit pageLogicalHeight = renderView->layoutState()->pageLogicalHeight();
        LayoutUnit remainingHeight = pageLogicalHeight - intMod(offset, pageLogicalHeight);
        if (pageBoundaryRule == IncludePageBoundary) {
            remainingHeight = intMod(remainingHeight, pageLogicalHeight);
        }
        return remainingHeight;
    }

    return flowThread->pageRemainingLogicalHeightForOffset(offset, pageBoundaryRule);
}
