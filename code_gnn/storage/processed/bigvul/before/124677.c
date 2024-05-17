LayoutUnit RenderBlockFlow::applyAfterBreak(RenderBox* child, LayoutUnit logicalOffset, MarginInfo& marginInfo)
{
    RenderFlowThread* flowThread = flowThreadContainingBlock();
    bool isInsideMulticolFlowThread = flowThread;
    bool checkColumnBreaks = isInsideMulticolFlowThread || view()->layoutState()->isPaginatingColumns();
    bool checkPageBreaks = !checkColumnBreaks && view()->layoutState()->pageLogicalHeight();  
    bool checkAfterAlways = (checkColumnBreaks && child->style()->columnBreakAfter() == PBALWAYS)
        || (checkPageBreaks && child->style()->pageBreakAfter() == PBALWAYS);
    if (checkAfterAlways && inNormalFlow(child)) {
        LayoutUnit marginOffset = marginInfo.canCollapseWithMarginBefore() ? LayoutUnit() : marginInfo.margin();

        marginInfo.clearMargin();

        if (checkColumnBreaks) {
            if (isInsideMulticolFlowThread) {
                LayoutUnit offsetBreakAdjustment = 0;
                if (flowThread->addForcedRegionBreak(offsetFromLogicalTopOfFirstPage() + logicalOffset + marginOffset, child, false, &offsetBreakAdjustment))
                    return logicalOffset + marginOffset + offsetBreakAdjustment;
            } else {
                view()->layoutState()->addForcedColumnBreak(*child, logicalOffset);
            }
        }
        return nextPageLogicalTop(logicalOffset, IncludePageBoundary);
    }
    return logicalOffset;
}
