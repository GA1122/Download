void LayoutSVGViewportContainer::paint(const PaintInfo& paintInfo, const LayoutPoint& paintOffset) const
{
    SVGContainerPainter(*this).paint(paintInfo);
}
