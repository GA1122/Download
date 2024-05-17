void WebPagePrivate::enterFullScreenForElement(Element* element)
{
#if ENABLE(VIDEO)
    if (!element || (!element->hasTagName(HTMLNames::videoTag) && !containsVideoTags(element)))
        return;
    if (m_webSettings->fullScreenVideoCapable()) {
        enterFullscreenForNode(element);
    } else {
        if (transformedActualVisibleSize() != transformedViewportSize()) {
            m_scaleBeforeFullScreen = currentScale();

            m_scrollPositionBeforeFullScreen = m_mainFrame->view()->scrollPosition();
        }

        client()->fullscreenStart();
        m_fullscreenVideoNode = element;
    }
#endif
}
