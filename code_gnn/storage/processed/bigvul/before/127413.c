PassRefPtr<RenderStyle> StyleResolver::styleForPage(int pageIndex)
{
    resetDirectionAndWritingModeOnDocument(document());
    StyleResolverState state(document(), document().documentElement());  

    state.setStyle(RenderStyle::create());
    const RenderStyle* rootElementStyle = state.rootElementStyle() ? state.rootElementStyle() : document().renderStyle();
    ASSERT(rootElementStyle);
    state.style()->inheritFrom(rootElementStyle);

    state.fontBuilder().initForStyleResolve(state.document(), state.style(), state.useSVGZoomRules());

    PageRuleCollector collector(rootElementStyle, pageIndex);

    collector.matchPageRules(CSSDefaultStyleSheets::defaultPrintStyle);

    if (ScopedStyleResolver* scopedResolver = m_styleTree.scopedStyleResolverForDocument())
        scopedResolver->matchPageRules(collector);

    state.setLineHeightValue(0);
    bool inheritedOnly = false;

    MatchResult& result = collector.matchedResult();
    applyMatchedProperties<VariableDefinitions>(state, result, false, 0, result.matchedProperties.size() - 1, inheritedOnly);
    applyMatchedProperties<HighPriorityProperties>(state, result, false, 0, result.matchedProperties.size() - 1, inheritedOnly);

    updateFont(state);

    if (state.lineHeightValue())
        StyleBuilder::applyProperty(CSSPropertyLineHeight, state, state.lineHeightValue());

    applyMatchedProperties<LowPriorityProperties>(state, result, false, 0, result.matchedProperties.size() - 1, inheritedOnly);

    addContentAttrValuesToFeatures(state.contentAttrValues(), m_features);

    m_styleResourceLoader.loadPendingResources(state.style(), state.elementStyleResources());

    didAccess();

    return state.takeStyle();
}
