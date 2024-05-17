LayoutUnit LayoutBlockFlow::clearFloatsIfNeeded(LayoutBox& child, MarginInfo& marginInfo, LayoutUnit oldTopPosMargin, LayoutUnit oldTopNegMargin, LayoutUnit yPos, bool childIsSelfCollapsing, bool childDiscardMargin)
{
    LayoutUnit heightIncrease = getClearDelta(&child, yPos);
    marginInfo.setLastChildIsSelfCollapsingBlockWithClearance(false);

    if (!heightIncrease)
        return yPos;

    if (childIsSelfCollapsing) {
        marginInfo.setLastChildIsSelfCollapsingBlockWithClearance(true);
        marginInfo.setDiscardMargin(childDiscardMargin);

        LayoutBlockFlow::MarginValues childMargins = marginValuesForChild(child);
        if (!childDiscardMargin) {
            marginInfo.setPositiveMargin(std::max(childMargins.positiveMarginBefore(), childMargins.positiveMarginAfter()));
            marginInfo.setNegativeMargin(std::max(childMargins.negativeMarginBefore(), childMargins.negativeMarginAfter()));
        } else {
            marginInfo.clearMargin();
        }

        marginInfo.setCanCollapseMarginAfterWithLastChild(false);

        setLogicalHeight(child.logicalTop() + childMargins.negativeMarginBefore());
    } else {
        setLogicalHeight(logicalHeight() + heightIncrease);
    }

    if (marginInfo.canCollapseWithMarginBefore()) {
        setMaxMarginBeforeValues(oldTopPosMargin, oldTopNegMargin);
        marginInfo.setAtBeforeSideOfBlock(false);

        setMustDiscardMarginBefore(style()->marginBeforeCollapse() == MDISCARD);
    }

    return yPos + heightIncrease;
}
