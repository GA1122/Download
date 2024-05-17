void WebPagePrivate::adjustFullScreenElementDimensionsIfNeeded()
{
    if (!m_fullscreenNode || !m_fullscreenNode->renderer()
        || !m_fullscreenNode->document() || !m_fullscreenNode->document()->fullScreenRenderer())
        return;

    ASSERT(m_fullscreenNode->isElementNode());
    ASSERT(static_cast<Element*>(m_fullscreenNode.get())->isMediaElement());

    Document* document = m_fullscreenNode->document();
    RenderStyle* fullScreenStyle = document->fullScreenRenderer()->style();
    ASSERT(fullScreenStyle);

    IntRect viewportRect = m_mainFrame->view()->visibleContentRect();
    int viewportWidth = viewportRect.width() + 1;
    int viewportHeight = viewportRect.height() + 1;

    fullScreenStyle->setWidth(Length(viewportWidth, WebCore::Fixed));
    fullScreenStyle->setHeight(Length(viewportHeight, WebCore::Fixed));
    fullScreenStyle->setLeft(Length(0, WebCore::Fixed));
    fullScreenStyle->setTop(Length(0, WebCore::Fixed));
    fullScreenStyle->setBackgroundColor(Color::black);

    document->fullScreenRenderer()->setNeedsLayoutAndPrefWidthsRecalc();
    document->recalcStyle(Node::Force);
}
