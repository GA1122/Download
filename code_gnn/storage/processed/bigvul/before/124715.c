LayoutPoint RenderBlockFlow::flipFloatForWritingModeForChild(const FloatingObject* child, const LayoutPoint& point) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return point;

    if (isHorizontalWritingMode())
        return LayoutPoint(point.x(), point.y() + height() - child->renderer()->height() - 2 * yPositionForFloatIncludingMargin(child));
    return LayoutPoint(point.x() + width() - child->renderer()->width() - 2 * xPositionForFloatIncludingMargin(child), point.y());
}
