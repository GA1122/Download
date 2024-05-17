RenderBox* SVGImage::embeddedContentBox() const
{
    if (!m_page)
        return 0;
    LocalFrame* frame = m_page->mainFrame();
    SVGSVGElement* rootElement = toSVGDocument(frame->document())->rootElement();
    if (!rootElement)
        return 0;
    return toRenderBox(rootElement->renderer());
}
