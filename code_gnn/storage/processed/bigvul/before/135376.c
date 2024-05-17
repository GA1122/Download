void Document::setParsingState(ParsingState parsingState)
{
    m_parsingState = parsingState;

    if (parsing() && !m_elementDataCache)
        m_elementDataCache = ElementDataCache::create();
}
