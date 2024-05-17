void Document::detachParser()
{
    if (!m_parser)
        return;
    m_parser->detach();
    m_parser.clear();
    DocumentParserTiming::from(*this).markParserDetached();
}
