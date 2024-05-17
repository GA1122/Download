void SVGDocumentExtensions::unregisterSVGFontFaceElement(SVGFontFaceElement* element)
{
    ASSERT(m_svgFontFaceElements.contains(element));
    m_svgFontFaceElements.remove(element);
}
