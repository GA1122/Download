bool RenderBox::getBackgroundPaintedExtent(LayoutRect& paintedExtent) const
{
    ASSERT(hasBackground());
    LayoutRect backgroundRect = pixelSnappedIntRect(borderBoxRect());

    Color backgroundColor = resolveColor(CSSPropertyBackgroundColor);
    if (backgroundColor.alpha()) {
        paintedExtent = backgroundRect;
        return true;
    }

    if (!style()->backgroundLayers()->image() || style()->backgroundLayers()->next()) {
        paintedExtent =  backgroundRect;
        return true;
    }

    BackgroundImageGeometry geometry;
    calculateBackgroundImageGeometry(0, style()->backgroundLayers(), backgroundRect, geometry);
    if (geometry.hasNonLocalGeometry())
        return false;
    paintedExtent = geometry.destRect();
    return true;
}
