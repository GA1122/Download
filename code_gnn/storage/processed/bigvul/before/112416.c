PassRefPtr<Element> Document::createElement(const QualifiedName& qName, bool createdByParser)
{
    RefPtr<Element> e;

    if (qName.namespaceURI() == xhtmlNamespaceURI)
        e = HTMLElementFactory::createHTMLElement(qName, this, 0, createdByParser);
#if ENABLE(SVG)
    else if (qName.namespaceURI() == SVGNames::svgNamespaceURI)
        e = SVGElementFactory::createSVGElement(qName, this, createdByParser);
#endif
#if ENABLE(MATHML)
    else if (qName.namespaceURI() == MathMLNames::mathmlNamespaceURI)
        e = MathMLElementFactory::createMathMLElement(qName, this, createdByParser);
#endif

    if (e)
        m_sawElementsInKnownNamespaces = true;
    else
        e = Element::create(qName, document());

    ASSERT((qName.matches(imageTag) && e->tagQName().matches(imgTag) && e->tagQName().prefix() == qName.prefix()) || qName == e->tagQName());

    return e.release();
}
