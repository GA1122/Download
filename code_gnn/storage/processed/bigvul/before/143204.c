DocumentParser* Document::implicitOpen(ParserSynchronizationPolicy parserSyncPolicy)
{
    detachParser();

    removeChildren();
    DCHECK(!m_focusedElement);

    setCompatibilityMode(NoQuirksMode);

    if (!threadedParsingEnabledForTesting())
        parserSyncPolicy = ForceSynchronousParsing;

    m_parserSyncPolicy = parserSyncPolicy;
    m_parser = createParser();
    DocumentParserTiming::from(*this).markParserStart();
    setParsingState(Parsing);
    setReadyState(Loading);

    return m_parser;
}
