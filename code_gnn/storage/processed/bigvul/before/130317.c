void RuleFeatureSet::add(const RuleFeatureSet& other)
{
    for (const auto& entry : other.m_classInvalidationSets)
        ensureClassInvalidationData(entry.key).combine(*entry.value);
    for (const auto& entry : other.m_attributeInvalidationSets)
        ensureAttributeInvalidationData(entry.key).combine(*entry.value);
    for (const auto& entry : other.m_idInvalidationSets)
        ensureIdInvalidationData(entry.key).combine(*entry.value);
    for (const auto& entry : other.m_pseudoInvalidationSets)
        ensurePseudoInvalidationData(static_cast<CSSSelector::PseudoType>(entry.key)).combine(*entry.value);

    m_metadata.add(other.m_metadata);

    siblingRules.appendVector(other.siblingRules);
    uncommonAttributeRules.appendVector(other.uncommonAttributeRules);
}
