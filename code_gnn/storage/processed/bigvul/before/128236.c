IntRect FrameView::convertToRenderer(const RenderObject& renderer, const IntRect& viewRect) const
{
    IntRect rect = viewRect;

    rect.moveBy(scrollPosition());

    rect.setLocation(roundedIntPoint(renderer.absoluteToLocal(rect.location(), UseTransforms)));
    return rect;
}
