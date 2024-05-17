void Document::adjustFloatRectForScrollAndAbsoluteZoom(FloatRect& rect, RenderObject* renderer)
{
    if (!view())
        return;

    LayoutRect visibleContentRect = view()->visibleContentRect();
    rect.move(-visibleContentRect.x(), -visibleContentRect.y());
    adjustFloatRectForAbsoluteZoom(rect, renderer);
}
