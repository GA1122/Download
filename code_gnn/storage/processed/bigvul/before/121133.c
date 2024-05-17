PassRefPtr<HTMLInputElement> HTMLInputElement::create(const QualifiedName& tagName, Document* document, HTMLFormElement* form, bool createdByParser)
{
    RefPtr<HTMLInputElement> inputElement = adoptRef(new HTMLInputElement(tagName, document, form, createdByParser));
    inputElement->ensureUserAgentShadowRoot();
    return inputElement.release();
}
