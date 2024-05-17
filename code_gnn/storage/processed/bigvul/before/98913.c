PassRefPtr<Element> HTMLConstructionSite::createHTMLElementFromSavedElement(Element* element)
{

    ASSERT(element->isHTMLElement());  
    AtomicHTMLToken fakeToken(HTMLToken::StartTag, element->localName(), cloneAttributes(element));
    return createHTMLElement(fakeToken);
}
