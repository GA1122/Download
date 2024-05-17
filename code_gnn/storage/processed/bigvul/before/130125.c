PassRefPtr<HTMLBodyElement> HTMLBodyElement::create(Document& document)
{
    return adoptRef(new HTMLBodyElement(document));
}
