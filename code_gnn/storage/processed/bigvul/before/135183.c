PassRefPtrWillBeRawPtr<Element> Document::createElement(const QualifiedName& qName, bool createdByParser)
{
    RefPtrWillBeRawPtr<Element> e = nullptr;

    if (qName.namespaceURI() == xhtmlNamespaceURI)
        e = HTMLElementFactory::createHTMLElement(qName.localName(), *this, 0, createdByParser);
    else if (qName.namespaceURI() == SVGNames::svgNamespaceURI)
        e = SVGElementFactory::createSVGElement(qName.localName(), *this, createdByParser);

    if (e)
        m_sawElementsInKnownNamespaces = true;
    else
        e = Element::create(qName, this);

    if (e->prefix() != qName.prefix())
        e->setTagNameForCreateElementNS(qName);

    ASSERT(qName == e->tagQName());

    return e.release();
}
