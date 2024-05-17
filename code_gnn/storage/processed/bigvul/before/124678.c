LayoutUnit RenderBlockFlow::applyBeforeBreak(RenderBox* child, LayoutUnit logicalOffset)
{
    RenderFlowThread* flowThread = flowThreadContainingBlock();
    bool isInsideMulticolFlowThread = flowThread;
    bool checkColumnBreaks = isInsideMulticolFlowThread || view()->layoutState()->isPaginatingColumns();
    bool checkPageBreaks = !checkColumnBreaks && view()->layoutState()->pageLogicalHeight();  
    bool checkBeforeAlways = (checkColumnBreaks && child->style()->columnBreakBefore() == PBALWAYS)
        || (checkPageBreaks && child->style()->pageBreakBefore() == PBALWAYS);
    if (checkBeforeAlways && inNormalFlow(child)) {
        if (checkColumnBreaks) {
            if (isInsideMulticolFlowThread) {
                LayoutUnit offsetBreakAdjustment = 0;
                if (flowThread->addForcedRegionBreak(offsetFromLogicalTopOfFirstPage() + logicalOffset, child, true, &offsetBreakAdjustment))
                    return logicalOffset + offsetBreakAdjustment;
            } else {
                view()->layoutState()->addForcedColumnBreak(*child, logicalOffset);
            }
        }
        return nextPageLogicalTop(logicalOffset, IncludePageBoundary);
    }
    return logicalOffset;
}
