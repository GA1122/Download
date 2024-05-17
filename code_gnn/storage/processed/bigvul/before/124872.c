LayoutPoint RenderBox::flipForWritingModeForChild(const RenderBox* child, const LayoutPoint& point) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return point;

    if (isHorizontalWritingMode())
        return LayoutPoint(point.x(), point.y() + height() - child->height() - (2 * child->y()));
    return LayoutPoint(point.x() + width() - child->width() - (2 * child->x()), point.y());
}
