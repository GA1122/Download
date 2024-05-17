void LayoutSVGViewportContainer::determineIfLayoutSizeChanged()
{
    ASSERT(element());
    if (!isSVGSVGElement(*element()))
        return;

    m_isLayoutSizeChanged = toSVGSVGElement(element())->hasRelativeLengths() && selfNeedsLayout();
}
