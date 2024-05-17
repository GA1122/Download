bool LayoutSVGResourceMarker::shouldPaint() const
{
    SVGMarkerElement* marker = toSVGMarkerElement(element());
    ASSERT(marker);
    return !marker->viewBox()->isSpecified()
        || !marker->viewBox()->currentValue()->isValid()
        || !marker->viewBox()->currentValue()->value().isEmpty();
}
