FloatRect SVGLayoutSupport::localOverflowRectForPaintInvalidation(const LayoutObject& object)
{
    ASSERT(!object.isSVGRoot());

    if (object.styleRef().visibility() != VISIBLE && !object.enclosingLayer()->hasVisibleContent())
        return FloatRect();

    FloatRect paintInvalidationRect = object.paintInvalidationRectInLocalSVGCoordinates();
    if (int outlineOutset = object.styleRef().outlineOutsetExtent())
        paintInvalidationRect.inflate(outlineOutset);
    return paintInvalidationRect;
}
