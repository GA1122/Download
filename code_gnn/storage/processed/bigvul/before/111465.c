void InputHandler::ensureFocusPluginElementVisible()
{
    if (!isActivePlugin() || !m_currentFocusElement->document())
        return;

    Frame* elementFrame = m_currentFocusElement->document()->frame();
    if (!elementFrame)
        return;

    Frame* mainFrame = m_webPage->mainFrame();
    if (!mainFrame)
        return;

    FrameView* mainFrameView = mainFrame->view();
    if (!mainFrameView)
        return;

    WebCore::IntRect selectionFocusRect;

    RenderWidget* renderWidget = static_cast<RenderWidget*>(m_currentFocusElement->renderer());
    if (renderWidget) {
        PluginView* pluginView = static_cast<PluginView*>(renderWidget->widget());

        if (pluginView)
            selectionFocusRect = pluginView->ensureVisibleRect();
    }

    if (selectionFocusRect.isEmpty())
        return;

    if (elementFrame != mainFrame) {  
        selectionFocusRect.move(-elementFrame->view()->scrollPosition().x(), -elementFrame->view()->scrollPosition().y());

        if (elementFrame->ownerRenderer()) {
            WebCore::IntPoint frameOffset = elementFrame->ownerRenderer()->absoluteContentBox().location();
            selectionFocusRect.move(frameOffset.x(), frameOffset.y());
        }
    }

    WebCore::IntRect actualScreenRect = WebCore::IntRect(mainFrameView->scrollPosition(), m_webPage->actualVisibleSize());
    if (actualScreenRect.contains(selectionFocusRect))
        return;

    WebCore::IntPoint pos(selectionFocusRect.center().x() - actualScreenRect.width() / 2,
                 selectionFocusRect.center().y() - actualScreenRect.height() / 2);

    mainFrameView->setScrollPosition(pos);
}
