void StyleResolver::appendAuthorStyleSheets(unsigned firstNew, const Vector<RefPtr<CSSStyleSheet> >& styleSheets)
{
    unsigned size = styleSheets.size();
    for (unsigned i = firstNew; i < size; ++i) {
        CSSStyleSheet* cssSheet = styleSheets[i].get();
        ASSERT(!cssSheet->disabled());
        if (cssSheet->mediaQueries() && !m_medium->eval(cssSheet->mediaQueries(), &m_viewportDependentMediaQueryResults))
            continue;

        StyleSheetContents* sheet = cssSheet->contents();
        ContainerNode* scopingNode = ScopedStyleResolver::scopingNodeFor(cssSheet);
        if (!scopingNode && cssSheet->ownerNode() && cssSheet->ownerNode()->isInShadowTree())
            continue;

        ScopedStyleResolver* resolver = ensureScopedStyleResolver(scopingNode);
        ASSERT(resolver);
        resolver->addRulesFromSheet(sheet, *m_medium, this);
        m_inspectorCSSOMWrappers.collectFromStyleSheetIfNeeded(cssSheet);
    }
}
