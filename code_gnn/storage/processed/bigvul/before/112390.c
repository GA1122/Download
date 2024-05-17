void Document::adjustFloatRectForScrollAndAbsoluteZoomAndFrameScale(FloatRect& rect, RenderObject* renderer)
{
    if (!view())
        return;

    float inverseFrameScale = 1;
    if (frame())
        inverseFrameScale = 1 / frame()->frameScaleFactor();

    LayoutRect visibleContentRect = view()->visibleContentRect();
    rect.move(-visibleContentRect.x(), -visibleContentRect.y());
    adjustFloatRectForAbsoluteZoom(rect, renderer);
    if (inverseFrameScale != 1)
        rect.scale(inverseFrameScale);
}
