FloatRect LayoutSVGResourceMarker::markerBoundaries(const AffineTransform& markerTransformation) const
{
    FloatRect coordinates = LayoutSVGContainer::paintInvalidationRectInLocalSVGCoordinates();

    coordinates = localToSVGParentTransform().mapRect(coordinates);

    return markerTransformation.mapRect(coordinates);
}
