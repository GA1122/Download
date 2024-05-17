void LayoutSVGContainer::paint(const PaintInfo& paintInfo, const LayoutPoint&) const
{
    SVGContainerPainter(*this).paint(paintInfo);
}
