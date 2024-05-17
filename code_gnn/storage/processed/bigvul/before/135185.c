PassRefPtrWillBeRawPtr<Element> Document::createElementNS(const AtomicString& namespaceURI, const AtomicString& qualifiedName, const AtomicString& typeExtension, ExceptionState& exceptionState)
{
    QualifiedName qName(createQualifiedName(namespaceURI, qualifiedName, exceptionState));
    if (qName == QualifiedName::null())
        return nullptr;

    RefPtrWillBeRawPtr<Element> element;
    if (CustomElement::isValidName(qName.localName()) && registrationContext())
        element = registrationContext()->createCustomTagElement(*this, qName);
    else
        element = createElement(qName, false);

    if (!typeExtension.isEmpty())
        CustomElementRegistrationContext::setIsAttributeAndTypeExtension(element.get(), typeExtension);

    return element.release();
}
