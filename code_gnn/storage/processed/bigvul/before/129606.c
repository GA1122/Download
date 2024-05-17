bool LayoutSVGViewportContainer::pointIsInsideViewportClip(const FloatPoint& pointInParent)
{
    if (!SVGLayoutSupport::isOverflowHidden(this))
        return true;

    return m_viewport.contains(pointInParent);
}
