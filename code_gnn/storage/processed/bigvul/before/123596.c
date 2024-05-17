bool InspectorPageAgent::applyViewportStyleOverride(StyleResolver* resolver)
{
    if (!m_deviceMetricsOverridden || !m_emulateViewportEnabled)
        return false;

    RefPtr<StyleSheetContents> styleSheet = StyleSheetContents::create(CSSParserContext(UASheetMode));
    styleSheet->parseString(String(viewportAndroidUserAgentStyleSheet, sizeof(viewportAndroidUserAgentStyleSheet)));
    OwnPtr<RuleSet> ruleSet = RuleSet::create();
    ruleSet->addRulesFromSheet(styleSheet.get(), MediaQueryEvaluator("screen"));
    resolver->viewportStyleResolver()->collectViewportRules(ruleSet.get(), ViewportStyleResolver::UserAgentOrigin);
    return true;
}
