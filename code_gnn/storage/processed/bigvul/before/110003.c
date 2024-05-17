PassRefPtr<HTMLSelectElement> HTMLSelectElement::create(Document* document)
{
    return adoptRef(new HTMLSelectElement(selectTag, document, 0, false));
}
