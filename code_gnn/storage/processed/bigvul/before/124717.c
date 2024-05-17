void RenderBlockFlow::handleAfterSideOfBlock(RenderBox* lastChild, LayoutUnit beforeSide, LayoutUnit afterSide, MarginInfo& marginInfo)
{
    marginInfo.setAtAfterSideOfBlock(true);

    if (lastChild && lastChild->isRenderBlockFlow() && lastChild->isSelfCollapsingBlock())
        setLogicalHeight(logicalHeight() - toRenderBlockFlow(lastChild)->marginOffsetForSelfCollapsingBlock());

    if (marginInfo.canCollapseMarginAfterWithChildren() && !marginInfo.canCollapseMarginAfterWithLastChild())
        marginInfo.setCanCollapseMarginAfterWithChildren(false);

    if (!marginInfo.discardMargin() && (!marginInfo.canCollapseWithMarginAfter() && !marginInfo.canCollapseWithMarginBefore()
        && (!document().inQuirksMode() || !marginInfo.quirkContainer() || !marginInfo.hasMarginAfterQuirk())))
        setLogicalHeight(logicalHeight() + marginInfo.margin());

    setLogicalHeight(logicalHeight() + afterSide);

    setLogicalHeight(max(logicalHeight(), beforeSide + afterSide));

    setCollapsedBottomMargin(marginInfo);
}
