PassRefPtr<Element> Document::createElementNS(const AtomicString& namespaceURI, const String& qualifiedName, const AtomicString& typeExtension, ExceptionState& es)
{
    String prefix, localName;
    if (!parseQualifiedName(qualifiedName, prefix, localName, es))
        return 0;

    QualifiedName qName(prefix, localName, namespaceURI);
    if (!hasValidNamespaceForElements(qName)) {
        es.throwUninformativeAndGenericDOMException(NamespaceError);
        return 0;
    }

    RefPtr<Element> element;
    if (CustomElement::isValidName(qName.localName()) && registrationContext())
        element = registrationContext()->createCustomTagElement(*this, qName);
    else
        element = createElementNS(namespaceURI, qualifiedName, es);

    if (!typeExtension.isNull() && !typeExtension.isEmpty())
        CustomElementRegistrationContext::setIsAttributeAndTypeExtension(element.get(), typeExtension);

    return element;
}
