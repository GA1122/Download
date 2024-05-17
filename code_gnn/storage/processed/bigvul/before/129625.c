void SVGLayoutSupport::layoutChildren(
    LayoutObject* firstChild, bool forceLayout, bool screenScalingFactorChanged, bool layoutSizeChanged)
{
    for (LayoutObject* child = firstChild; child; child = child->nextSibling()) {
        bool forceChildLayout = forceLayout;

        if (screenScalingFactorChanged) {
            if (child->isSVGText())
                toLayoutSVGText(child)->setNeedsTextMetricsUpdate();
            forceChildLayout = true;
        }

        if (layoutSizeChanged) {
            if (SVGElement* element = child->node()->isSVGElement() ? toSVGElement(child->node()) : 0) {
                if (element->hasRelativeLengths()) {
                    if (child->isSVGShape()) {
                        toLayoutSVGShape(child)->setNeedsShapeUpdate();
                    } else if (child->isSVGText()) {
                        toLayoutSVGText(child)->setNeedsTextMetricsUpdate();
                        toLayoutSVGText(child)->setNeedsPositioningValuesUpdate();
                    }

                    forceChildLayout = true;
                }
            }
        }

        if (child->isSVGResourceContainer()) {
            layoutResourcesIfNeeded(child);
            child->layoutIfNeeded();
        } else {
            SubtreeLayoutScope layoutScope(*child);
            if (forceChildLayout)
                layoutScope.setNeedsLayout(child, LayoutInvalidationReason::SvgChanged);

            layoutResourcesIfNeeded(child);
            child->layoutIfNeeded();
        }
    }
}
