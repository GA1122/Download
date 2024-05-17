Element* Document::createElement(const AtomicString& localName, const AtomicString& typeExtension, ExceptionState& exceptionState)
{
    if (!isValidName(localName)) {
        exceptionState.throwDOMException(InvalidCharacterError, "The tag name provided ('" + localName + "') is not a valid name.");
        return nullptr;
    }

    Element* element;

    if (CustomElement::shouldCreateCustomElement(*this, localName)) {
        element = CustomElement::createCustomElementSync(*this, localName, exceptionState);
    } else if (V0CustomElement::isValidName(localName) && registrationContext()) {
        element = registrationContext()->createCustomTagElement(*this, QualifiedName(nullAtom, convertLocalName(localName), xhtmlNamespaceURI));
    } else {
        element = createElement(localName, exceptionState);
        if (exceptionState.hadException())
            return nullptr;
    }

    if (!typeExtension.isEmpty())
        V0CustomElementRegistrationContext::setIsAttributeAndTypeExtension(element, typeExtension);

    return element;
}
