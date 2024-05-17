void SVGDocumentExtensions::startPan(const FloatPoint& start)
{
    if (SVGSVGElement* svg = rootElement(*m_document))
        m_translate = FloatPoint(start.x() - svg->currentTranslate().x(), start.y() - svg->currentTranslate().y());
}
