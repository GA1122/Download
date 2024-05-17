const LayoutSVGRoot* SVGLayoutSupport::findTreeRootObject(const LayoutObject* start)
{
    while (start && !start->isSVGRoot())
        start = start->parent();

    ASSERT(start);
    ASSERT(start->isSVGRoot());
    return toLayoutSVGRoot(start);
}
