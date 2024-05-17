LayoutUnit RenderBlock::inlineDirectionOffset(const LayoutSize& offsetFromBlock) const
{
    return isHorizontalWritingMode() ? offsetFromBlock.width() : offsetFromBlock.height();
}
