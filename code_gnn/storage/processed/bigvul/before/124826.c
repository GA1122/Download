void RenderBox::computeBlockDirectionMargins(const RenderBlock* containingBlock, LayoutUnit& marginBefore, LayoutUnit& marginAfter) const
{
    if (isTableCell()) {
        marginBefore = 0;
        marginAfter = 0;
        return;
    }

    LayoutUnit cw = containingBlockLogicalWidthForContent();
    RenderStyle* containingBlockStyle = containingBlock->style();
    marginBefore = minimumValueForLength(style()->marginBeforeUsing(containingBlockStyle), cw);
    marginAfter = minimumValueForLength(style()->marginAfterUsing(containingBlockStyle), cw);
}
