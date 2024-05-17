IntPoint FrameView::convertToRenderer(const RenderObject& renderer, const IntPoint& viewPoint) const
{
    IntPoint point = viewPoint;

    point += IntSize(scrollX(), scrollY());

    return roundedIntPoint(renderer.absoluteToLocal(point, UseTransforms));
}
