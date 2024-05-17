bool RenderBlock::isSelfCollapsingBlock() const
{
    ASSERT(!needsLayout() || (node() && node()->isElementNode() && toElement(node())->shadowPseudoId() == "-webkit-input-placeholder"));


    if (createsBlockFormattingContext())
        return false;

    if (logicalHeight() > 0
        || isTable() || borderAndPaddingLogicalHeight()
        || style()->logicalMinHeight().isPositive()
        || style()->marginBeforeCollapse() == MSEPARATE || style()->marginAfterCollapse() == MSEPARATE)
        return false;

    Length logicalHeightLength = style()->logicalHeight();
    bool hasAutoHeight = logicalHeightLength.isAuto();
    if (logicalHeightLength.isPercent() && !document().inQuirksMode()) {
        hasAutoHeight = true;
        for (RenderBlock* cb = containingBlock(); !cb->isRenderView(); cb = cb->containingBlock()) {
            if (cb->style()->logicalHeight().isFixed() || cb->isTableCell())
                hasAutoHeight = false;
        }
    }

    if (hasAutoHeight || ((logicalHeightLength.isFixed() || logicalHeightLength.isPercent()) && logicalHeightLength.isZero())) {
        if (childrenInline())
            return !firstLineBox();

        if (m_hasOnlySelfCollapsingChildren)
            return true;
        for (RenderBox* child = firstChildBox(); child; child = child->nextSiblingBox()) {
            if (child->isFloatingOrOutOfFlowPositioned())
                continue;
            if (!child->isSelfCollapsingBlock())
                return false;
        }
        return true;
    }
    return false;
}
