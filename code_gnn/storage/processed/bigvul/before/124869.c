LayoutUnit RenderBox::flipForWritingMode(LayoutUnit position) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return position;
    return logicalHeight() - position;
}
