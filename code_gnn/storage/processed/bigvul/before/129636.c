LayoutRect SVGLayoutSupport::transformPaintInvalidationRect(const LayoutObject& object, const AffineTransform& rootTransform, const FloatRect& localRect)
{
    FloatRect adjustedRect = rootTransform.mapRect(localRect);

    if (object.isSVGShape() && object.styleRef().svgStyle().hasStroke()) {
        if (float strokeWidthForHairlinePadding = toLayoutSVGShape(object).strokeWidth()) {
            const FloatSize strokeSize = rootTransform.mapSize(
                FloatSize(strokeWidthForHairlinePadding, strokeWidthForHairlinePadding));
            if (strokeSize.width() < 1 || strokeSize.height() < 1) {
                const float pad = 0.5f - std::min(strokeSize.width(), strokeSize.height()) / 2;
                ASSERT(pad > 0);
                adjustedRect.inflate(pad);
            }
        }
    }

    if (adjustedRect.isEmpty())
        return LayoutRect();

    return LayoutRect(enclosingIntRect(adjustedRect));
}
