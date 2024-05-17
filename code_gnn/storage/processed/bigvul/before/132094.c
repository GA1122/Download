void LayoutBlockFlow::marginBeforeEstimateForChild(LayoutBox& child, LayoutUnit& positiveMarginBefore, LayoutUnit& negativeMarginBefore, bool& discardMarginBefore) const
{
    if ((document().inQuirksMode() && hasMarginBeforeQuirk(&child) && (isTableCell() || isBody())) || child.style()->marginBeforeCollapse() == MSEPARATE)
        return;

    if (child.style()->marginBeforeCollapse() == MDISCARD) {
        positiveMarginBefore = 0;
        negativeMarginBefore = 0;
        discardMarginBefore = true;
        return;
    }

    LayoutUnit beforeChildMargin = marginBeforeForChild(child);
    positiveMarginBefore = std::max(positiveMarginBefore, beforeChildMargin);
    negativeMarginBefore = std::max(negativeMarginBefore, -beforeChildMargin);

    if (!child.isLayoutBlockFlow())
        return;

    LayoutBlockFlow* childBlockFlow = toLayoutBlockFlow(&child);
    if (childBlockFlow->childrenInline() || childBlockFlow->isWritingModeRoot())
        return;

    MarginInfo childMarginInfo(childBlockFlow, childBlockFlow->borderBefore() + childBlockFlow->paddingBefore(), childBlockFlow->borderAfter() + childBlockFlow->paddingAfter());
    if (!childMarginInfo.canCollapseMarginBeforeWithChildren())
        return;

    LayoutBox* grandchildBox = childBlockFlow->firstChildBox();
    for ( ; grandchildBox; grandchildBox = grandchildBox->nextSiblingBox()) {
        if (!grandchildBox->isFloatingOrOutOfFlowPositioned() && !grandchildBox->isColumnSpanAll())
            break;
    }

    if (!grandchildBox)
        return;

    if (grandchildBox->needsLayout()) {
        grandchildBox->computeAndSetBlockDirectionMargins(this);
        if (grandchildBox->isLayoutBlock()) {
            LayoutBlock* grandchildBlock = toLayoutBlock(grandchildBox);
            grandchildBlock->setHasMarginBeforeQuirk(grandchildBox->style()->hasMarginBeforeQuirk());
            grandchildBlock->setHasMarginAfterQuirk(grandchildBox->style()->hasMarginAfterQuirk());
        }
    }

    if (grandchildBox->style()->clear() != CNONE && childBlockFlow->marginBeforeForChild(*grandchildBox) == 0)
        return;

    childBlockFlow->marginBeforeEstimateForChild(*grandchildBox, positiveMarginBefore, negativeMarginBefore, discardMarginBefore);
}
