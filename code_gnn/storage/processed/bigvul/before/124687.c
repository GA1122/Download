LayoutUnit RenderBlockFlow::clearFloatsIfNeeded(RenderBox* child, MarginInfo& marginInfo, LayoutUnit oldTopPosMargin, LayoutUnit oldTopNegMargin, LayoutUnit yPos, bool childIsSelfCollapsing)
{
    LayoutUnit heightIncrease = getClearDelta(child, yPos);
    if (!heightIncrease)
        return yPos;

    if (childIsSelfCollapsing) {
        bool childDiscardMargin = mustDiscardMarginBeforeForChild(child) || mustDiscardMarginAfterForChild(child);

        RenderBlockFlow::MarginValues childMargins = marginValuesForChild(child);
        if (!childDiscardMargin) {
            marginInfo.setPositiveMargin(max(childMargins.positiveMarginBefore(), childMargins.positiveMarginAfter()));
            marginInfo.setNegativeMargin(max(childMargins.negativeMarginBefore(), childMargins.negativeMarginAfter()));
        } else {
            marginInfo.clearMargin();
        }
        marginInfo.setDiscardMargin(childDiscardMargin);

        marginInfo.setCanCollapseMarginAfterWithLastChild(false);

        setLogicalHeight(child->logicalTop() + childMargins.negativeMarginBefore());
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
