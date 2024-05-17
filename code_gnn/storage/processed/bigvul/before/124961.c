bool RenderBox::skipContainingBlockForPercentHeightCalculation(const RenderBox* containingBlock) const
{
    if (containingBlock->isRenderFlowThread())
        return true;

    if (!document().inQuirksMode() && !containingBlock->isAnonymousBlock())
        return false;
    return !containingBlock->isTableCell() && !containingBlock->isOutOfFlowPositioned() && containingBlock->style()->logicalHeight().isAuto() && isHorizontalWritingMode() == containingBlock->isHorizontalWritingMode();
}
