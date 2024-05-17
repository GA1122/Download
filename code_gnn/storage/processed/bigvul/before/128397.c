LayoutRect RenderLayerScrollableArea::exposeRect(const LayoutRect& rect, const ScrollAlignment& alignX, const ScrollAlignment& alignY)
{
    LayoutRect localExposeRect(box().absoluteToLocalQuad(FloatQuad(FloatRect(rect)), UseTransforms).boundingBox());
    LayoutRect layerBounds(0, 0, box().clientWidth(), box().clientHeight());
    LayoutRect r = ScrollAlignment::getRectToExpose(layerBounds, localExposeRect, alignX, alignY);

    IntSize clampedScrollOffset = clampScrollOffset(adjustedScrollOffset() + toIntSize(roundedIntRect(r).location()));
    if (clampedScrollOffset == adjustedScrollOffset())
        return rect;

    IntSize oldScrollOffset = adjustedScrollOffset();
    scrollToOffset(clampedScrollOffset);
    IntSize scrollOffsetDifference = adjustedScrollOffset() - oldScrollOffset;
    localExposeRect.move(-scrollOffsetDifference);
    return LayoutRect(box().localToAbsoluteQuad(FloatQuad(FloatRect(localExposeRect)), UseTransforms).boundingBox());
}
