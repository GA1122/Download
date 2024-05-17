void SVGImage::startAnimation(bool  )
{
    if (!m_page)
        return;
    LocalFrame* frame = m_page->mainFrame();
    SVGSVGElement* rootElement = toSVGDocument(frame->document())->rootElement();
    if (!rootElement)
        return;
    rootElement->unpauseAnimations();
    rootElement->setCurrentTime(0);
}
