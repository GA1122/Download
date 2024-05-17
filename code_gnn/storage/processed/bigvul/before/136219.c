KURL getSubResourceURLFromElement(Element* element)
{
    ASSERT(element);
    const QualifiedName& attributeName = element->subResourceAttributeName();
    if (attributeName == QualifiedName::null())
        return KURL();

    String value = element->getAttribute(attributeName);
    if (value.isEmpty() || value.stripWhiteSpace().startsWith("javascript:", TextCaseInsensitive))
        return KURL();

    return element->document().completeURL(value);
}
