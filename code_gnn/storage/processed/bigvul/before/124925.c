bool RenderBox::percentageLogicalHeightIsResolvableFromBlock(const RenderBlock* containingBlock, bool isOutOfFlowPositioned)
{
    const RenderBlock* cb = containingBlock;
    bool inQuirksMode = cb->document().inQuirksMode();
    while (!cb->isRenderView() && !cb->isBody() && !cb->isTableCell() && !cb->isOutOfFlowPositioned() && cb->style()->logicalHeight().isAuto()) {
        if (!inQuirksMode && !cb->isAnonymousBlock())
            break;
        cb = cb->containingBlock();
    }

    bool isOutOfFlowPositionedWithSpecifiedHeight = cb->isOutOfFlowPositioned() && (!cb->style()->logicalHeight().isAuto() || (!cb->style()->top().isAuto() && !cb->style()->bottom().isAuto()));

    if (cb->isTableCell())
        return true;

    if (cb->style()->logicalHeight().isFixed())
        return true;
    if (cb->style()->logicalHeight().isPercent() && !isOutOfFlowPositionedWithSpecifiedHeight)
        return percentageLogicalHeightIsResolvableFromBlock(cb->containingBlock(), cb->isOutOfFlowPositioned());
    if (cb->isRenderView() || inQuirksMode || isOutOfFlowPositionedWithSpecifiedHeight)
        return true;
    if (cb->isDocumentElement() && isOutOfFlowPositioned) {
        return true;
    }

    return false;
}
