void WebPagePrivate::updateViewportSize(bool setFixedReportedSize, bool sendResizeEvent)
{
    if (!m_backingStore)
        return;
    ASSERT(m_mainFrame->view());
    IntSize visibleSize = actualVisibleSize();
    if (setFixedReportedSize)
        m_mainFrame->view()->setFixedReportedSize(visibleSize);

    IntRect frameRect = IntRect(scrollPosition(), visibleSize);
    if (frameRect != m_mainFrame->view()->frameRect()) {
        m_mainFrame->view()->setFrameRect(frameRect);
        m_mainFrame->view()->adjustViewSize();

#if ENABLE(FULLSCREEN_API)
        if (m_fullscreenVideoNode) {
            Document* document = m_fullscreenVideoNode->document();
            ASSERT(document);
            ASSERT(document->fullScreenRenderer());

            int width = m_mainFrame->view()->visibleContentRect().size().width();
            document->fullScreenRenderer()->style()->setWidth(Length(width, Fixed));
        }
#endif
    }

    if (sendResizeEvent && shouldSendResizeEvent())
        m_mainFrame->eventHandler()->sendResizeEvent();

    m_mainFrame->view()->repaintFixedElementsAfterScrolling();
}
