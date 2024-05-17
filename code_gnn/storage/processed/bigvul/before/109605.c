PassRefPtr<Element> Document::createElement(const AtomicString& localName, const AtomicString& typeExtension, ExceptionState& es)
{
    if (!isValidName(localName)) {
        es.throwUninformativeAndGenericDOMException(InvalidCharacterError);
        return 0;
    }

    RefPtr<Element> element;

    if (RuntimeEnabledFeatures::customElementsEnabled() && CustomElement::isValidName(localName) && registrationContext())
        element = registrationContext()->createCustomTagElement(*this, QualifiedName(nullAtom, localName, xhtmlNamespaceURI));
    else
        element = createElement(localName, es);

    if (RuntimeEnabledFeatures::customElementsEnabled() && !typeExtension.isNull() && !typeExtension.isEmpty())
        CustomElementRegistrationContext::setIsAttributeAndTypeExtension(element.get(), typeExtension);

    return element;
}
