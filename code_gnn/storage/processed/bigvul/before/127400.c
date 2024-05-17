void StyleResolver::processScopedRules(const RuleSet& authorRules, const KURL& sheetBaseURL, ContainerNode* scope)
{
    const Vector<StyleRuleKeyframes*> keyframesRules = authorRules.keyframesRules();
    for (unsigned i = 0; i < keyframesRules.size(); ++i)
        ensureScopedStyleResolver(scope)->addKeyframeStyle(keyframesRules[i]);

    const Vector<StyleRuleHost*> hostRules = authorRules.hostRules();
    if (hostRules.size() && scope && scope->isInShadowTree()) {
        bool enabled = buildScopedStyleTreeInDocumentOrder();
        setBuildScopedStyleTreeInDocumentOrder(false);
        bool hasDocumentSecurityOrigin = document().securityOrigin()->canRequest(sheetBaseURL);
        for (unsigned i = 0; i < hostRules.size(); ++i)
            ensureScopedStyleResolver(scope->shadowHost())->addHostRule(hostRules[i], hasDocumentSecurityOrigin, scope);
        setBuildScopedStyleTreeInDocumentOrder(enabled);
    }

    addTreeBoundaryCrossingRules(authorRules.treeBoundaryCrossingRules(), scope);

    if (!scope || scope->isDocumentNode()) {
        const Vector<StyleRuleFontFace*> fontFaceRules = authorRules.fontFaceRules();
        for (unsigned i = 0; i < fontFaceRules.size(); ++i)
            fontSelector()->addFontFaceRule(fontFaceRules[i]);
        if (fontFaceRules.size())
            invalidateMatchedPropertiesCache();
    } else {
        addTreeBoundaryCrossingRules(authorRules.shadowDistributedRules(), scope);
    }
}
