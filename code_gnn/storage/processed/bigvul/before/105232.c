HTMLCollection* HTMLElement::children()
{
    return ensureCachedHTMLCollection(NodeChildren);
}
