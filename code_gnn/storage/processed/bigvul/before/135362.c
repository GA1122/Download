void Document::setCompatibilityMode(CompatibilityMode mode)
{
    if (m_compatibilityModeLocked || mode == m_compatibilityMode)
        return;
    m_compatibilityMode = mode;
    selectorQueryCache().invalidate();
}
