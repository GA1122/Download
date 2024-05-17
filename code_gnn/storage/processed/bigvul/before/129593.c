FloatPoint LayoutSVGResourceMarker::referencePoint() const
{
    SVGMarkerElement* marker = toSVGMarkerElement(element());
    ASSERT(marker);

    SVGLengthContext lengthContext(marker);
    return FloatPoint(marker->refX()->currentValue()->value(lengthContext), marker->refY()->currentValue()->value(lengthContext));
}
