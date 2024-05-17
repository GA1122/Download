void LayoutBlockFlow::handleAfterSideOfBlock(LayoutBox* lastChild, LayoutUnit beforeSide, LayoutUnit afterSide, MarginInfo& marginInfo)
{
    marginInfo.setAtAfterSideOfBlock(true);

    if (marginInfo.lastChildIsSelfCollapsingBlockWithClearance()) {
        ASSERT(lastChild);
        setLogicalHeight(logicalHeight() - marginValuesForChild(*lastChild).positiveMarginBefore());
    }

    if (marginInfo.canCollapseMarginAfterWithChildren() && !marginInfo.canCollapseMarginAfterWithLastChild())
        marginInfo.setCanCollapseMarginAfterWithChildren(false);

    if (!marginInfo.discardMargin() && (!marginInfo.canCollapseWithMarginAfter() && !marginInfo.canCollapseWithMarginBefore()
        && (!document().inQuirksMode() || !marginInfo.quirkContainer() || !marginInfo.hasMarginAfterQuirk())))
        setLogicalHeight(logicalHeight() + marginInfo.margin());

    setLogicalHeight(logicalHeight() + afterSide);

    setLogicalHeight(std::max(logicalHeight(), beforeSide + afterSide));

    setCollapsedBottomMargin(marginInfo);
}
