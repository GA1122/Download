bool LayoutSVGContainer::selfWillPaint() const
{
    return SVGLayoutSupport::hasFilterResource(*this);
}
