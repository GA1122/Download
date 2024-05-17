LayoutUnit RenderBlock::textIndentOffset() const
{
    LayoutUnit cw = 0;
    if (style()->textIndent().isPercent())
        cw = containingBlock()->availableLogicalWidth();
    return minimumValueForLength(style()->textIndent(), cw);
}
