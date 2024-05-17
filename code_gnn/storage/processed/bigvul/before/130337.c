void RuleFeatureSet::updateInvalidationSets(const RuleData& ruleData)
{
    InvalidationSetFeatures features;
    auto result = extractInvalidationSetFeatures(ruleData.selector(), features, Subject);

    if (result.first) {
        features.forceSubtree = result.second == ForceSubtree;
        addFeaturesToInvalidationSets(result.first, features.adjacent ? &features : nullptr, features);
    }

    if (features.hasBeforeOrAfter)
        updateInvalidationSetsForContentAttribute(ruleData);
}
