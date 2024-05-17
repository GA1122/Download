StyleResolver::~StyleResolver()
{
    m_fontSelector->unregisterForInvalidationCallbacks(this);
    m_fontSelector->clearDocument();
    m_viewportStyleResolver->clearDocument();
}
