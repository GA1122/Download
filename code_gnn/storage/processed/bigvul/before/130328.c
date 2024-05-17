InvalidationData& RuleFeatureSet::ensureIdInvalidationData(const AtomicString& id)
{
    return ensureInvalidationData(m_idInvalidationSets, id);
}
