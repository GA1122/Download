bool Document::haveRenderBlockingStylesheetsLoaded() const
{
    if (RuntimeEnabledFeatures::cssInBodyDoesNotBlockPaintEnabled())
        return m_styleEngine->haveRenderBlockingStylesheetsLoaded();
    return m_styleEngine->haveScriptBlockingStylesheetsLoaded();
}
