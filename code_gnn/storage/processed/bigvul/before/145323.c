HTMLAnchorElement* HTMLAnchorElement::create(Document& document)
{
    return new HTMLAnchorElement(aTag, document);
}
