PassRefPtrWillBeRawPtr<DocumentParser> Document::implicitOpen(ParserSynchronizationPolicy parserSyncPolicy)
{
    cancelParsing();

    removeChildren();
    ASSERT(!m_focusedElement);

    setCompatibilityMode(NoQuirksMode);

    m_parserSyncPolicy = parserSyncPolicy;
    m_parser = createParser();
    setParsingState(Parsing);
    setReadyState(Loading);

    return m_parser;
}
