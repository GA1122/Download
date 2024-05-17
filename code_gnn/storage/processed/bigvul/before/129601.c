bool LayoutSVGTransformableContainer::isChildAllowed(LayoutObject* child, const ComputedStyle& style) const
{
    ASSERT(element());
    if (isSVGSwitchElement(*element())) {
        Node* node = child->node();
        if (!node->isSVGElement() || !toSVGElement(node)->isValid())
            return false;
        if (hasValidPredecessor(node))
            return false;
    } else if (isSVGAElement(*element())) {
        if (isSVGAElement(*child->node()))
            return false;
        if (parent() && parent()->isSVG())
            return parent()->isChildAllowed(child, style);
    }
     return LayoutSVGContainer::isChildAllowed(child, style);
 }
