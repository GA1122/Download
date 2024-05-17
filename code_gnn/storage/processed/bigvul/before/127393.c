void StyleResolver::matchAuthorRulesForShadowHost(Element* element, ElementRuleCollector& collector, bool includeEmptyRules, Vector<ScopedStyleResolver*, 8>& resolvers, Vector<ScopedStyleResolver*, 8>& resolversInShadowTree)
{
    collector.clearMatchedRules();
    collector.matchedResult().ranges.lastAuthorRule = collector.matchedResult().matchedProperties.size() - 1;

    CascadeScope cascadeScope = 0;
    CascadeOrder cascadeOrder = 0;
    bool applyAuthorStyles = applyAuthorStylesOf(element);

    for (int j = resolversInShadowTree.size() - 1; j >= 0; --j)
        resolversInShadowTree.at(j)->collectMatchingAuthorRules(collector, includeEmptyRules, applyAuthorStyles, cascadeScope, cascadeOrder++);

    if (resolvers.isEmpty() || resolvers.first()->treeScope() != element->treeScope())
        ++cascadeScope;
    cascadeOrder += resolvers.size();
    for (unsigned i = 0; i < resolvers.size(); ++i)
        resolvers.at(i)->collectMatchingAuthorRules(collector, includeEmptyRules, applyAuthorStyles, cascadeScope++, --cascadeOrder);

    collectTreeBoundaryCrossingRules(element, collector, includeEmptyRules);
    collector.sortAndTransferMatchedRules();

    if (!resolvers.isEmpty())
        matchHostRules(element, resolvers.first(), collector, includeEmptyRules);
}
