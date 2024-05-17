void StyleResolver::matchAuthorRules(Element* element, ElementRuleCollector& collector, bool includeEmptyRules)
{
    if (m_styleTree.hasOnlyScopedResolverForDocument()) {
        m_styleTree.scopedStyleResolverForDocument()->matchAuthorRules(collector, includeEmptyRules, applyAuthorStylesOf(element));

        collector.clearMatchedRules();
        collector.matchedResult().ranges.lastAuthorRule = collector.matchedResult().matchedProperties.size() - 1;
        collectTreeBoundaryCrossingRules(element, collector, includeEmptyRules);
        collector.sortAndTransferMatchedRules();
        return;
    }

    Vector<ScopedStyleResolver*, 8> resolvers;
    m_styleTree.resolveScopedStyles(element, resolvers);

    Vector<ScopedStyleResolver*, 8> resolversInShadowTree;
    m_styleTree.collectScopedResolversForHostedShadowTrees(element, resolversInShadowTree);
    if (!resolversInShadowTree.isEmpty()) {
        matchAuthorRulesForShadowHost(element, collector, includeEmptyRules, resolvers, resolversInShadowTree);
        return;
    }

    if (resolvers.isEmpty())
        return;

    bool applyAuthorStyles = applyAuthorStylesOf(element);
    CascadeScope cascadeScope = 0;
    CascadeOrder cascadeOrder = resolvers.size();
    collector.clearMatchedRules();
    collector.matchedResult().ranges.lastAuthorRule = collector.matchedResult().matchedProperties.size() - 1;

    for (unsigned i = 0; i < resolvers.size(); ++i, --cascadeOrder) {
        ScopedStyleResolver* resolver = resolvers.at(i);
        resolver->collectMatchingAuthorRules(collector, includeEmptyRules, applyAuthorStyles, cascadeScope++, resolver->treeScope() == element->treeScope() && resolver->scopingNode().isShadowRoot() ? 0 : cascadeOrder);
    }

    collectTreeBoundaryCrossingRules(element, collector, includeEmptyRules);
    collector.sortAndTransferMatchedRules();

    matchHostRules(element, resolvers.first(), collector, includeEmptyRules);
}
