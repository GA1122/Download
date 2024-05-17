SVGMarkerUnitsType LayoutSVGResourceMarker::markerUnits() const
{
    return toSVGMarkerElement(element())->markerUnits()->currentValue()->enumValue();
}
