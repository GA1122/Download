bool Document::hasActiveParser()
{
    return m_activeParserCount || (m_parser && m_parser->processingData());
}
