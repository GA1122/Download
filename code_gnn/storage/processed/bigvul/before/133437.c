void SVGImage::stopAnimation()
{
    if (!m_page)
        return;
    LocalFrame* frame = m_page->mainFrame();
    SVGSVGElement* rootElement = toSVGDocument(frame->document())->rootElement();
    if (!rootElement)
        return;
    rootElement->pauseAnimations();
}
