void LayoutSVGContainer::addChild(LayoutObject* child, LayoutObject* beforeChild)
{
    LayoutSVGModelObject::addChild(child, beforeChild);
    SVGResourcesCache::clientWasAddedToTree(child, child->styleRef());

    bool shouldIsolateDescendants = (child->isBlendingAllowed() && child->style()->hasBlendMode()) || child->hasNonIsolatedBlendingDescendants();
    if (shouldIsolateDescendants)
        descendantIsolationRequirementsChanged(DescendantIsolationRequired);
}
