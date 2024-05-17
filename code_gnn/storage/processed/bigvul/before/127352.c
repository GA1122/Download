void InspectorCSSOMWrappers::collectFromStyleSheetIfNeeded(CSSStyleSheet* styleSheet)
{
    if (!m_styleRuleToCSSOMWrapperMap.isEmpty())
        collect(styleSheet);
}
