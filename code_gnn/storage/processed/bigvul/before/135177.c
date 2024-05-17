PassRefPtrWillBeRawPtr<CDATASection> Document::createCDATASection(const String& data, ExceptionState& exceptionState)
{
    if (isHTMLDocument()) {
        exceptionState.throwDOMException(NotSupportedError, "This operation is not supported for HTML documents.");
        return nullptr;
    }
    if (data.contains("]]>")) {
        exceptionState.throwDOMException(InvalidCharacterError, "String cannot contain ']]>' since that is the end delimiter of a CData section.");
        return nullptr;
    }
    return CDATASection::create(*this, data);
}
