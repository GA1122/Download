void RuleFeatureSet::addFeaturesToInvalidationSet(InvalidationSet& invalidationSet, const InvalidationSetFeatures& features)
{
    if (features.treeBoundaryCrossing)
        invalidationSet.setTreeBoundaryCrossing();
    if (features.insertionPointCrossing)
        invalidationSet.setInsertionPointCrossing();
    if (features.forceSubtree) {
        invalidationSet.setWholeSubtreeInvalid();
        return;
    }
    if (!features.id.isEmpty())
        invalidationSet.addId(features.id);
    if (!features.tagName.isEmpty())
        invalidationSet.addTagName(features.tagName);
    for (const auto& className : features.classes)
        invalidationSet.addClass(className);
    for (const auto& attribute : features.attributes)
        invalidationSet.addAttribute(attribute);
    if (features.customPseudoElement)
        invalidationSet.setCustomPseudoInvalid();
}
