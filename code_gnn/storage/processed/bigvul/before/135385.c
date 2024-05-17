void Document::setXMLStandalone(bool standalone, ExceptionState& exceptionState)
{
    m_xmlStandalone = standalone ? Standalone : NotStandalone;
}
