void SVGDocumentExtensions::updatePan(const FloatPoint& pos) const
{
    if (SVGSVGElement* svg = rootElement(*m_document))
        svg->setCurrentTranslate(FloatPoint(pos.x() - m_translate.x(), pos.y() - m_translate.y()));
}
