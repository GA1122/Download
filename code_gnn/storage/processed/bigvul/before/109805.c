void Document::setContent(const String& content)
{
    open();
    m_parser->pinToMainThread();
    m_parser->append(content.impl());
    close();
}
