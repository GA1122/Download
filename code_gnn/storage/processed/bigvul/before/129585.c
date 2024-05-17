float LayoutSVGResourceMarker::angle() const
{
    return toSVGMarkerElement(element())->orientAngle()->currentValue()->value();
}
