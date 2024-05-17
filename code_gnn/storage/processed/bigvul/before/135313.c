void Document::notifyResizeForViewportUnits()
{
    if (m_mediaQueryMatcher)
        m_mediaQueryMatcher->viewportChanged();
    if (!hasViewportUnits())
        return;
    ensureStyleResolver().notifyResizeForViewportUnits();
    setNeedsStyleRecalcForViewportUnits();
}
