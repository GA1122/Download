SVGSVGElement* SVGDocumentExtensions::rootElement(const Document& document)
{
    Element* elem = document.documentElement();
    return isSVGSVGElement(elem) ? toSVGSVGElement(elem) : 0;
}
