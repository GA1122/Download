SelectorQueryCache& Document::selectorQueryCache()
{
    if (!m_selectorQueryCache)
        m_selectorQueryCache = wrapUnique(new SelectorQueryCache());
    return *m_selectorQueryCache;
}
