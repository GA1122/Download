bool SVGLayoutSupport::screenScaleFactorChanged(const LayoutObject* ancestor)
{
    for (; ancestor; ancestor = ancestor->parent()) {
        if (ancestor->isSVGRoot())
            return toLayoutSVGRoot(ancestor)->didScreenScaleFactorChange();
        if (ancestor->isSVGTransformableContainer())
            return toLayoutSVGTransformableContainer(ancestor)->didScreenScaleFactorChange();
        if (ancestor->isSVGViewportContainer())
            return toLayoutSVGViewportContainer(ancestor)->didScreenScaleFactorChange();
    }
    ASSERT_NOT_REACHED();
    return false;
}
