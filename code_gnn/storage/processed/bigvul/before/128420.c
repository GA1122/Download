IntSize RenderLayerScrollableArea::offsetFromResizeCorner(const IntPoint& absolutePoint) const
{
    IntSize elementSize = layer()->size();
    if (box().style()->shouldPlaceBlockDirectionScrollbarOnLogicalLeft())
        elementSize.setWidth(0);
    IntPoint resizerPoint = IntPoint(elementSize);
    IntPoint localPoint = roundedIntPoint(box().absoluteToLocal(absolutePoint, UseTransforms));
    return localPoint - resizerPoint;
}
