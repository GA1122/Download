LayoutUnit LayoutBlockFlow::applyBeforeBreak(LayoutBox& child, LayoutUnit logicalOffset)
{
    LayoutFlowThread* flowThread = flowThreadContainingBlock();
    bool checkColumnBreaks = flowThread;
    bool checkPageBreaks = !checkColumnBreaks && view()->layoutState()->pageLogicalHeight();  
    bool checkBeforeAlways = (checkColumnBreaks && child.style()->columnBreakBefore() == PBALWAYS)
        || (checkPageBreaks && child.style()->pageBreakBefore() == PBALWAYS);
    if (checkBeforeAlways && inNormalFlow(&child)) {
        if (checkColumnBreaks) {
            LayoutUnit offsetBreakAdjustment = 0;
            if (flowThread->addForcedColumnBreak(offsetFromLogicalTopOfFirstPage() + logicalOffset, &child, true, &offsetBreakAdjustment))
                return logicalOffset + offsetBreakAdjustment;
        }
        return nextPageLogicalTop(logicalOffset, AssociateWithFormerPage);
    }
    return logicalOffset;
}
