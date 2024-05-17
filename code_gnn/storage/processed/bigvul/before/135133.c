void Document::adjustFloatRectForScrollAndAbsoluteZoom(FloatRect& rect, LayoutObject& layoutObject)
{
    if (!view())
        return;

    LayoutRect visibleContentRect(view()->visibleContentRect());
    rect.move(-FloatSize(visibleContentRect.x().toFloat(), visibleContentRect.y().toFloat()));
    adjustFloatRectForAbsoluteZoom(rect, layoutObject);
}
