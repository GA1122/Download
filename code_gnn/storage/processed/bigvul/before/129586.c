void LayoutSVGResourceMarker::calcViewport()
{
    if (!selfNeedsLayout())
        return;

    SVGMarkerElement* marker = toSVGMarkerElement(element());
    ASSERT(marker);

    SVGLengthContext lengthContext(marker);
    float w = marker->markerWidth()->currentValue()->value(lengthContext);
    float h = marker->markerHeight()->currentValue()->value(lengthContext);
     m_viewport = FloatRect(0, 0, w, h);
 }
