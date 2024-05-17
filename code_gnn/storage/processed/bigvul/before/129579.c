void LayoutSVGContainer::removeChild(LayoutObject* child)
{
    SVGResourcesCache::clientWillBeRemovedFromTree(child);
    LayoutSVGModelObject::removeChild(child);

    bool hadNonIsolatedDescendants = (child->isBlendingAllowed() && child->style()->hasBlendMode()) || child->hasNonIsolatedBlendingDescendants();
    if (hadNonIsolatedDescendants)
        descendantIsolationRequirementsChanged(DescendantIsolationNeedsUpdate);
}
