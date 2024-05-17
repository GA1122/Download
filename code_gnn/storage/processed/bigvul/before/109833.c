void Document::setXMLVersion(const String& version, ExceptionState& es)
{
    if (!implementation()->hasFeature("XML", String())) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return;
    }

    if (!XMLDocumentParser::supportsXMLVersion(version)) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return;
    }

    m_xmlVersion = version;
}
