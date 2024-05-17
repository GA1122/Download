void Document::setFullScreenRendererSize(const IntSize& size)
{
    ASSERT(m_fullScreenRenderer);
    if (!m_fullScreenRenderer)
        return;
    
    if (m_fullScreenRenderer) {
        RefPtr<RenderStyle> newStyle = RenderStyle::clone(m_fullScreenRenderer->style());
        newStyle->setWidth(Length(size.width(), WebCore::Fixed));
        newStyle->setHeight(Length(size.height(), WebCore::Fixed));
        newStyle->setTop(Length(0, WebCore::Fixed));
        newStyle->setLeft(Length(0, WebCore::Fixed));
        m_fullScreenRenderer->setStyle(newStyle);
        updateLayout();
    }
}
