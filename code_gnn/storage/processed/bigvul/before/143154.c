Element* Document::createElement(const QualifiedName& qName, CreateElementFlags flags)
{
    Element* e = nullptr;

    if (qName.namespaceURI() == xhtmlNamespaceURI)
        e = HTMLElementFactory::createHTMLElement(qName.localName(), *this, 0, flags);
    else if (qName.namespaceURI() == SVGNames::svgNamespaceURI)
        e = SVGElementFactory::createSVGElement(qName.localName(), *this, flags);

    if (e)
        m_sawElementsInKnownNamespaces = true;
    else
        e = Element::create(qName, this);

    if (e->prefix() != qName.prefix())
        e->setTagNameForCreateElementNS(qName);

    DCHECK(qName == e->tagQName());

    return e;
}
