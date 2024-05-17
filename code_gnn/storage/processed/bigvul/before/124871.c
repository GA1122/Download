LayoutSize RenderBox::flipForWritingMode(const LayoutSize& offset) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return offset;
    return isHorizontalWritingMode() ? LayoutSize(offset.width(), height() - offset.height()) : LayoutSize(width() - offset.width(), offset.height());
}
