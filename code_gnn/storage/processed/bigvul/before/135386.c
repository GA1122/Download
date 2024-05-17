void Document::setXMLVersion(const String& version, ExceptionState& exceptionState)
{
    if (!XMLDocumentParser::supportsXMLVersion(version)) {
        exceptionState.throwDOMException(NotSupportedError, "This document does not support the XML version '" + version + "'.");
        return;
    }

    m_xmlVersion = version;
}
