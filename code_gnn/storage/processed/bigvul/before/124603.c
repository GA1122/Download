LayoutUnit RenderBlock::pageLogicalTopForOffset(LayoutUnit offset) const
{
    RenderView* renderView = view();
    LayoutUnit firstPageLogicalTop = isHorizontalWritingMode() ? renderView->layoutState()->pageOffset().height() : renderView->layoutState()->pageOffset().width();
    LayoutUnit blockLogicalTop = isHorizontalWritingMode() ? renderView->layoutState()->layoutOffset().height() : renderView->layoutState()->layoutOffset().width();

    LayoutUnit cumulativeOffset = offset + blockLogicalTop;
    RenderFlowThread* flowThread = flowThreadContainingBlock();
    if (!flowThread) {
        LayoutUnit pageLogicalHeight = renderView->layoutState()->pageLogicalHeight();
        if (!pageLogicalHeight)
            return 0;
        return cumulativeOffset - roundToInt(cumulativeOffset - firstPageLogicalTop) % roundToInt(pageLogicalHeight);
    }
    return flowThread->pageLogicalTopForOffset(cumulativeOffset);
}
