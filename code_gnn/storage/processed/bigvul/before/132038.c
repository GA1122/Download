void LayoutBlockFlow::adjustPositionedBlock(LayoutBox& child, const MarginInfo& marginInfo)
{
    LayoutUnit logicalTop = logicalHeight();
    updateStaticInlinePositionForChild(child, logicalTop);

    if (!marginInfo.canCollapseWithMarginBefore()) {
        LayoutUnit collapsedBeforePos = marginInfo.positiveMargin();
        LayoutUnit collapsedBeforeNeg = marginInfo.negativeMargin();
        logicalTop += collapsedBeforePos - collapsedBeforeNeg;
    }

    PaintLayer* childLayer = child.layer();
    if (childLayer->staticBlockPosition() != logicalTop)
        childLayer->setStaticBlockPosition(logicalTop);
}
