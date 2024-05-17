LayoutRect LayoutBlockFlow::selectionRectForPaintInvalidation(const LayoutBoxModelObject* paintInvalidationContainer) const
{
    LayoutRect rect = selectionGapRectsForPaintInvalidation(paintInvalidationContainer);
    if (paintInvalidationContainer->layer()->groupedMapping())
        PaintLayer::mapRectToPaintBackingCoordinates(paintInvalidationContainer, rect);
    return rect;
}
