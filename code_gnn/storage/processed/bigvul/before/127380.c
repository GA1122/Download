void StyleResolver::fontsNeedUpdate(FontSelector* fontSelector)
{
    invalidateMatchedPropertiesCache();
    m_document.setNeedsStyleRecalc();
}
