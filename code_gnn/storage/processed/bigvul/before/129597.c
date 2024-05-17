AffineTransform LayoutSVGResourceMarker::viewportTransform() const
{
    SVGMarkerElement* marker = toSVGMarkerElement(element());
    ASSERT(marker);

    return marker->viewBoxToViewTransform(m_viewport.width(), m_viewport.height());
}
