DocumentParser* Document::createParser()
{
    if (isHTMLDocument())
        return HTMLDocumentParser::create(toHTMLDocument(*this), m_parserSyncPolicy);
    return XMLDocumentParser::create(*this, view());
}
