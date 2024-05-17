PassRefPtr<CSSRuleList> StyleResolver::cssRulesForElement(Element* element, unsigned rulesToInclude, ShouldIncludeStyleSheetInCSSOMWrapper includeDocument)
{
    return pseudoCSSRulesForElement(element, NOPSEUDO, rulesToInclude, includeDocument);
}
