inline void StyleResolver::collectTreeBoundaryCrossingRules(Element* element, ElementRuleCollector& collector, bool includeEmptyRules)
{
    if (m_treeBoundaryCrossingRules.isEmpty())
        return;

    bool previousCanUseFastReject = collector.canUseFastReject();
    collector.setCanUseFastReject(false);

    RuleRange ruleRange = collector.matchedResult().ranges.authorRuleRange();

    CascadeOrder cascadeOrder = 0;

    DocumentOrderedList::iterator it = m_treeBoundaryCrossingRules.end();
    while (it != m_treeBoundaryCrossingRules.begin()) {
        --it;
        const ContainerNode* scopingNode = toContainerNode(*it);
        RuleSet* ruleSet = m_treeBoundaryCrossingRules.ruleSetScopedBy(scopingNode);
        unsigned boundaryBehavior = SelectorChecker::CrossesBoundary | SelectorChecker::ScopeContainsLastMatchedElement;

        if (scopingNode && scopingNode->isShadowRoot()) {
            if (element->isInDescendantTreeOf(toShadowRoot(scopingNode)->host()))
                boundaryBehavior |= SelectorChecker::ScopeIsShadowHost;
            scopingNode = toShadowRoot(scopingNode)->host();
        }
        collector.collectMatchingRules(MatchRequest(ruleSet, includeEmptyRules, scopingNode), ruleRange, static_cast<SelectorChecker::BehaviorAtBoundary>(boundaryBehavior), ignoreCascadeScope, cascadeOrder++);
    }
    collector.setCanUseFastReject(previousCanUseFastReject);
}
