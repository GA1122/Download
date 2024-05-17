LayoutUnit LayoutBlockFlow::applyAfterBreak(LayoutBox& child, LayoutUnit logicalOffset, MarginInfo& marginInfo)
{
    LayoutFlowThread* flowThread = flowThreadContainingBlock();
    bool checkColumnBreaks = flowThread;
    bool checkPageBreaks = !checkColumnBreaks && view()->layoutState()->pageLogicalHeight();  
    bool checkAfterAlways = (checkColumnBreaks && child.style()->columnBreakAfter() == PBALWAYS)
        || (checkPageBreaks && child.style()->pageBreakAfter() == PBALWAYS);
    if (checkAfterAlways && inNormalFlow(&child)) {
        marginInfo.clearMargin();

        if (checkColumnBreaks) {
            LayoutUnit offsetBreakAdjustment = 0;
            if (flowThread->addForcedColumnBreak(offsetFromLogicalTopOfFirstPage() + logicalOffset, &child, false, &offsetBreakAdjustment))
                return logicalOffset + offsetBreakAdjustment;
        }
        return nextPageLogicalTop(logicalOffset, AssociateWithFormerPage);
    }
    return logicalOffset;
}
