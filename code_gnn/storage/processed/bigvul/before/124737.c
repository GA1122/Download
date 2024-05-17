void RenderBlockFlow::marginBeforeEstimateForChild(RenderBox* child, LayoutUnit& positiveMarginBefore, LayoutUnit& negativeMarginBefore, bool& discardMarginBefore) const
{
    if ((document().inQuirksMode() && hasMarginBeforeQuirk(child) && (isTableCell() || isBody())) || child->style()->marginBeforeCollapse() == MSEPARATE)
        return;

    if (child->style()->marginBeforeCollapse() == MDISCARD) {
        positiveMarginBefore = 0;
        negativeMarginBefore = 0;
        discardMarginBefore = true;
        return;
    }

    LayoutUnit beforeChildMargin = marginBeforeForChild(child);
    positiveMarginBefore = max(positiveMarginBefore, beforeChildMargin);
    negativeMarginBefore = max(negativeMarginBefore, -beforeChildMargin);

    if (!child->isRenderBlockFlow())
        return;

    RenderBlockFlow* childBlockFlow = toRenderBlockFlow(child);
    if (childBlockFlow->childrenInline() || childBlockFlow->isWritingModeRoot())
        return;

    MarginInfo childMarginInfo(childBlockFlow, childBlockFlow->borderBefore() + childBlockFlow->paddingBefore(), childBlockFlow->borderAfter() + childBlockFlow->paddingAfter());
    if (!childMarginInfo.canCollapseMarginBeforeWithChildren())
        return;

    RenderBox* grandchildBox = childBlockFlow->firstChildBox();
    for ( ; grandchildBox; grandchildBox = grandchildBox->nextSiblingBox()) {
        if (!grandchildBox->isFloatingOrOutOfFlowPositioned())
            break;
    }

    if (!grandchildBox || grandchildBox->style()->clear() != CNONE)
        return;

    if (grandchildBox->needsLayout()) {
        grandchildBox->computeAndSetBlockDirectionMargins(this);
        if (grandchildBox->isRenderBlock()) {
            RenderBlock* grandchildBlock = toRenderBlock(grandchildBox);
            grandchildBlock->setHasMarginBeforeQuirk(grandchildBox->style()->hasMarginBeforeQuirk());
            grandchildBlock->setHasMarginAfterQuirk(grandchildBox->style()->hasMarginAfterQuirk());
        }
    }

    childBlockFlow->marginBeforeEstimateForChild(grandchildBox, positiveMarginBefore, negativeMarginBefore, discardMarginBefore);
}
