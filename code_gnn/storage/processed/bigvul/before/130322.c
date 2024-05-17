void RuleFeatureSet::collectFeaturesFromRuleData(const RuleData& ruleData)
{
    updateInvalidationSets(ruleData);

    FeatureMetadata metadata;
    collectFeaturesFromSelector(ruleData.selector(), metadata);
    m_metadata.add(metadata);

    if (metadata.foundSiblingSelector)
        siblingRules.append(RuleFeature(ruleData.rule(), ruleData.selectorIndex(), ruleData.hasDocumentSecurityOrigin()));
    if (ruleData.containsUncommonAttributeSelector())
        uncommonAttributeRules.append(RuleFeature(ruleData.rule(), ruleData.selectorIndex(), ruleData.hasDocumentSecurityOrigin()));
}
