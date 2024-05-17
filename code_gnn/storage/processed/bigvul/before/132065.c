LayoutPoint LayoutBlockFlow::flipFloatForWritingModeForChild(const FloatingObject& child, const LayoutPoint& point) const
{
    if (!style()->isFlippedBlocksWritingMode())
        return point;

    if (isHorizontalWritingMode())
        return LayoutPoint(point.x(), point.y() + size().height() - child.layoutObject()->size().height() - 2 * yPositionForFloatIncludingMargin(child));
    return LayoutPoint(point.x() + size().width() - child.layoutObject()->size().width() - 2 * xPositionForFloatIncludingMargin(child), point.y());
}
