StyleResolver& Document::ensureStyleResolver() const
{
    return m_styleEngine->ensureResolver();
}
