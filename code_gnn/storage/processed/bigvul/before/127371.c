void StyleResolver::collectFeatures()
{
    m_features.clear();
    if (CSSDefaultStyleSheets::defaultStyle)
        m_features.add(CSSDefaultStyleSheets::defaultStyle->features());

    if (document().isViewSource())
        m_features.add(CSSDefaultStyleSheets::viewSourceStyle()->features());

    if (m_watchedSelectorsRules)
        m_features.add(m_watchedSelectorsRules->features());

    m_treeBoundaryCrossingRules.collectFeaturesTo(m_features);

    m_styleTree.collectFeaturesTo(m_features);

    m_siblingRuleSet = makeRuleSet(m_features.siblingRules);
    m_uncommonAttributeRuleSet = makeRuleSet(m_features.uncommonAttributeRules);
}
