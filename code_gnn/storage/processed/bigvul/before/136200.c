void LinkChangeSerializerMarkupAccumulator::appendAttribute(StringBuilder& result, const Element& element, const Attribute& attribute, Namespaces* namespaces)
{
    if (!m_rewriteURLs.isEmpty() && element.isURLAttribute(attribute)) {

        String completeURL = document().completeURL(attribute.value());

        if (m_rewriteURLs.contains(completeURL)) {
            result.append(' ');
            result.append(attribute.name().toString());
            result.appendLiteral("=\"");
            if (!m_rewriteFolder.isEmpty())
                MarkupFormatter::appendAttributeValue(result, m_rewriteFolder + "/", document().isHTMLDocument());
            MarkupFormatter::appendAttributeValue(result, m_rewriteURLs.get(completeURL), document().isHTMLDocument());
            result.appendLiteral("\"");
            return;
        }
    }
    MarkupAccumulator::appendAttribute(result, element, attribute, namespaces);
}
