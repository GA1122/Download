bool StyleResolver::hasRulesForId(const AtomicString& id) const
{
    return m_features.idsInRules.contains(id.impl());
}
