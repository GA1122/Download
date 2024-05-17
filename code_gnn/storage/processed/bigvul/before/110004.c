PassRefPtr<HTMLSelectElement> HTMLSelectElement::create(const QualifiedName& tagName, Document* document, HTMLFormElement* form, bool createdByParser)
{
    ASSERT(tagName.matches(selectTag));
    return adoptRef(new HTMLSelectElement(tagName, document, form, createdByParser));
}
