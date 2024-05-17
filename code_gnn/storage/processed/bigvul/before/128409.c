bool RenderLayerScrollableArea::isPointInResizeControl(const IntPoint& absolutePoint, ResizerHitTestType resizerHitTestType) const
{
    if (!box().canResize())
        return false;

    IntPoint localPoint = roundedIntPoint(box().absoluteToLocal(absolutePoint, UseTransforms));
    IntRect localBounds(0, 0, box().pixelSnappedWidth(), box().pixelSnappedHeight());
    return resizerCornerRect(localBounds, resizerHitTestType).contains(localPoint);
}
