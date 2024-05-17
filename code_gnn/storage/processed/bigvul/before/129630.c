bool SVGLayoutSupport::mapToVisualRectInAncestorSpace(const LayoutObject& object, const LayoutBoxModelObject* ancestor, const FloatRect& localPaintInvalidationRect, LayoutRect& resultRect, VisualRectFlags visualRectFlags)
{
    AffineTransform rootBorderBoxTransform;
    const LayoutSVGRoot& svgRoot = computeTransformToSVGRoot(object, rootBorderBoxTransform);
    resultRect = transformPaintInvalidationRect(object, rootBorderBoxTransform, localPaintInvalidationRect);

    if (svgRoot.shouldApplyViewportClip()) {
        LayoutRect clipRect(svgRoot.pixelSnappedBorderBoxRect());
        if (visualRectFlags & EdgeInclusive) {
            if (!resultRect.inclusiveIntersect(clipRect))
                return false;
        } else {
            resultRect.intersect(clipRect);
        }
    }
    return svgRoot.mapToVisualRectInAncestorSpace(ancestor, resultRect, visualRectFlags);
}
