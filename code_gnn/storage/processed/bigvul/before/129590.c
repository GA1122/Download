AffineTransform LayoutSVGResourceMarker::markerTransformation(const FloatPoint& origin, float autoAngle, float strokeWidth) const
{
    float markerScale = markerUnits() == SVGMarkerUnitsStrokeWidth ? strokeWidth : 1;

    AffineTransform transform;
    transform.translate(origin.x(), origin.y());
    transform.rotate(orientType() == SVGMarkerOrientAngle ? angle() : autoAngle);
    transform.scale(markerScale);

    FloatPoint mappedReferencePoint = viewportTransform().mapPoint(referencePoint());
    transform.translate(-mappedReferencePoint.x(), -mappedReferencePoint.y());
    return transform;
}
