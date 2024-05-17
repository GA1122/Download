static const LayoutSVGRoot& computeTransformToSVGRoot(const LayoutObject& object, AffineTransform& rootBorderBoxTransform)
{
    ASSERT(object.isSVG() && !object.isSVGRoot());

    const LayoutObject* parent;
    for (parent = &object; !parent->isSVGRoot(); parent = parent->parent())
        rootBorderBoxTransform.preMultiply(parent->localToSVGParentTransform());

    const LayoutSVGRoot& svgRoot = toLayoutSVGRoot(*parent);
    rootBorderBoxTransform.preMultiply(svgRoot.localToBorderBoxTransform());
    return svgRoot;
}
