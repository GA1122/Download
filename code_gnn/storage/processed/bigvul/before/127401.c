PassRefPtr<CSSRuleList> StyleResolver::pseudoCSSRulesForElement(Element* element, PseudoId pseudoId, unsigned rulesToInclude, ShouldIncludeStyleSheetInCSSOMWrapper includeDocument)
{
    ASSERT(element);
    StyleResolverState state(document(), element);
    ElementRuleCollector collector(state.elementContext(), m_selectorFilter, state.style(), includeDocument);
    collector.setMode(SelectorChecker::CollectingCSSRules);
    collectPseudoRulesForElement(element, collector, pseudoId, rulesToInclude);
    return collector.matchedCSSRuleList();
}
