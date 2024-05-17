LayoutPoint RenderBox::flipForWritingMode(const LayoutPoint& position) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return position;
    return isHorizontalWritingMode() ? LayoutPoint(position.x(), height() - position.y()) : LayoutPoint(width() - position.x(), position.y());
}
