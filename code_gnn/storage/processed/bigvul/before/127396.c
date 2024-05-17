void StyleResolver::matchWatchSelectorRules(ElementRuleCollector& collector)
{
    if (!m_watchedSelectorsRules)
        return;

    collector.clearMatchedRules();
    collector.matchedResult().ranges.lastUserRule = collector.matchedResult().matchedProperties.size() - 1;

    MatchRequest matchRequest(m_watchedSelectorsRules.get());
    RuleRange ruleRange = collector.matchedResult().ranges.userRuleRange();
    collector.collectMatchingRules(matchRequest, ruleRange);
    collector.collectMatchingRulesForRegion(matchRequest, ruleRange);

    collector.sortAndTransferMatchedRules();
}
