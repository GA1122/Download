SVGSVGElement* SVGDocumentExtensions::rootElement() const
{
    ASSERT(m_document);
    return rootElement(*m_document);
}
