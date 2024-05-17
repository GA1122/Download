InvalidationData& RuleFeatureSet::ensurePseudoInvalidationData(CSSSelector::PseudoType pseudoType)
{
    return ensureInvalidationData(m_pseudoInvalidationSets, pseudoType);
}
