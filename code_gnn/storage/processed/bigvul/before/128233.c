IntPoint FrameView::convertFromRenderer(const RenderObject& renderer, const IntPoint& rendererPoint) const
{
    IntPoint point = roundedIntPoint(renderer.localToAbsolute(rendererPoint, UseTransforms));

    point.moveBy(-scrollPosition());
    return point;
}
