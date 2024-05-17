void SerializerMarkupAccumulator::appendElement(StringBuilder& result, Element& element, Namespaces* namespaces)
{
    if (!shouldIgnoreElement(element))
        MarkupAccumulator::appendElement(result, element, namespaces);

    if (isHTMLHeadElement(element)) {
        result.appendLiteral("<meta http-equiv=\"Content-Type\" content=\"");
        MarkupFormatter::appendAttributeValue(result, m_document->suggestedMIMEType(), m_document->isHTMLDocument());
        result.appendLiteral("; charset=");
        MarkupFormatter::appendAttributeValue(result, m_document->characterSet(), m_document->isHTMLDocument());
        if (m_document->isXHTMLDocument())
            result.appendLiteral("\" />");
        else
            result.appendLiteral("\">");
    }

}
