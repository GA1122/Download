void Document::implicitOpen()
{
    cancelParsing();

    removeChildren();

    setCompatibilityMode(NoQuirksMode);

    if (shouldDisplaySeamlesslyWithParent())
        styleResolverChanged(DeferRecalcStyle);

    m_parser = createParser();
    setParsing(true);
    setReadyState(Loading);
}
