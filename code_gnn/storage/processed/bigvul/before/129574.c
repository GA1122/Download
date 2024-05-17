void LayoutSVGContainer::addOutlineRects(Vector<LayoutRect>& rects, const LayoutPoint&, IncludeBlockVisualOverflowOrNot) const
{
    rects.append(LayoutRect(paintInvalidationRectInLocalSVGCoordinates()));
}
