void Document::cancelParsing()
{
    if (!m_parser)
        return;

    detachParser();
    explicitClose();
}
