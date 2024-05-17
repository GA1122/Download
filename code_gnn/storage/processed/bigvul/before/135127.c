SVGDocumentExtensions& Document::accessSVGExtensions()
{
    if (!m_svgExtensions)
        m_svgExtensions = adoptPtrWillBeNoop(new SVGDocumentExtensions(this));
    return *m_svgExtensions;
}
