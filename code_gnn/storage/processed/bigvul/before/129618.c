LayoutObject* SVGLayoutSupport::findClosestLayoutSVGText(LayoutObject* layoutObject, const FloatPoint& point)
{
    return searchTreeForFindClosestLayoutSVGText(layoutObject, point).candidateLayoutObject;
}
