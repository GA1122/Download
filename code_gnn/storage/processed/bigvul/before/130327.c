InvalidationData& RuleFeatureSet::ensureClassInvalidationData(const AtomicString& className)
{
    return ensureInvalidationData(m_classInvalidationSets, className);
}
