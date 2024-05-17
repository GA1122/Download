void StyleResolver::matchHostRules(Element* element, ScopedStyleResolver* resolver, ElementRuleCollector& collector, bool includeEmptyRules)
{
    if (element != &resolver->scopingNode())
        return;
    resolver->matchHostRules(collector, includeEmptyRules);
}
