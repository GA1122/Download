void LayoutSVGContainer::styleDidChange(StyleDifference diff, const ComputedStyle* oldStyle)
{
    LayoutSVGModelObject::styleDidChange(diff, oldStyle);

    bool hadIsolation = oldStyle && !isSVGHiddenContainer() && SVGLayoutSupport::willIsolateBlendingDescendantsForStyle(*oldStyle);
    bool isolationChanged = hadIsolation == !SVGLayoutSupport::willIsolateBlendingDescendantsForObject(this);

    if (!parent() || !isolationChanged)
        return;

    if (hasNonIsolatedBlendingDescendants())
        parent()->descendantIsolationRequirementsChanged(SVGLayoutSupport::willIsolateBlendingDescendantsForObject(this) ? DescendantIsolationNeedsUpdate : DescendantIsolationRequired);
}
