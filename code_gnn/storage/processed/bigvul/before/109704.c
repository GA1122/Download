PassRefPtr<DocumentParser> Document::implicitOpen()
{
    cancelParsing();

    removeChildren();
    ASSERT(!m_focusedElement);

    setCompatibilityMode(NoQuirksMode);

    if (shouldDisplaySeamlesslyWithParent())
        styleResolverChanged(RecalcStyleDeferred);

    m_parser = createParser();
    setParsing(true);
    setReadyState(Loading);

    return m_parser;
}
