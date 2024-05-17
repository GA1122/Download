void Document::setContent(const String& content)
{
    open();
    m_parser->append(content);
    close();
}
