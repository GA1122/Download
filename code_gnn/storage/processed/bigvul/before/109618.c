void Document::createStyleResolver()
{
    bool matchAuthorAndUserStyles = true;
    if (Settings* docSettings = settings())
        matchAuthorAndUserStyles = docSettings->authorAndUserStylesEnabled();
    m_styleResolver = adoptPtr(new StyleResolver(*this, matchAuthorAndUserStyles));
    m_styleEngine->combineCSSFeatureFlags(m_styleResolver->ruleFeatureSet());
}
