LayoutUnit RenderBlock::blockDirectionOffset(const LayoutSize& offsetFromBlock) const
{
    return isHorizontalWritingMode() ? offsetFromBlock.height() : offsetFromBlock.width();
}
