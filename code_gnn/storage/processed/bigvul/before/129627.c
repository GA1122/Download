bool SVGLayoutSupport::layoutSizeOfNearestViewportChanged(const LayoutObject* start)
{
    for (; start; start = start->parent()) {
        if (start->isSVGRoot())
            return toLayoutSVGRoot(start)->isLayoutSizeChanged();
        if (start->isSVGViewportContainer())
            return toLayoutSVGViewportContainer(start)->isLayoutSizeChanged();
    }
    ASSERT_NOT_REACHED();
    return false;
}
