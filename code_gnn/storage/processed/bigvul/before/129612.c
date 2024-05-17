LayoutRect SVGLayoutSupport::clippedOverflowRectForPaintInvalidation(const LayoutObject& object, const LayoutBoxModelObject& paintInvalidationContainer)
{
    LayoutRect rect;
    mapToVisualRectInAncestorSpace(object, &paintInvalidationContainer, localOverflowRectForPaintInvalidation(object), rect);
    return rect;
}
