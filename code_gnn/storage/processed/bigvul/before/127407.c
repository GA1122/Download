void StyleResolver::resetFontSelector()
{
    ASSERT(m_fontSelector);
    m_fontSelector->unregisterForInvalidationCallbacks(this);
    m_fontSelector->clearDocument();
    invalidateMatchedPropertiesCache();

    m_fontSelector = CSSFontSelector::create(&m_document);
    m_fontSelector->registerForInvalidationCallbacks(this);
}
