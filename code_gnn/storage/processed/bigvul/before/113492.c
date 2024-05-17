bool WebPagePrivate::handleMouseEvent(PlatformMouseEvent& mouseEvent)
{
    EventHandler* eventHandler = m_mainFrame->eventHandler();

    if (mouseEvent.type() == WebCore::PlatformEvent::MouseMoved)
        return eventHandler->mouseMoved(mouseEvent);

    if (mouseEvent.type() == WebCore::PlatformEvent::MouseScroll)
        return true;

    Node* node = 0;
    if (mouseEvent.inputMethod() == TouchScreen) {
        const FatFingersResult lastFatFingersResult = m_touchEventHandler->lastFatFingersResult();

        node = lastFatFingersResult.node(FatFingersResult::ShadowContentNotAllowed);
    }

    if (!node) {
        HitTestResult result = eventHandler->hitTestResultAtPoint(mapFromViewportToContents(mouseEvent.position()));
        node = result.innerNode();
    }

    if (mouseEvent.type() == WebCore::PlatformEvent::MousePressed) {
        m_inputHandler->setInputModeEnabled();
        if (m_inputHandler->willOpenPopupForNode(node)) {

            ASSERT_WITH_SECURITY_IMPLICATION(node->isElementNode());
            if (node->isElementNode()) {
                Element* element = static_cast<Element*>(node);
                element->focus();
            }
        } else
            eventHandler->handleMousePressEvent(mouseEvent);
    } else if (mouseEvent.type() == WebCore::PlatformEvent::MouseReleased) {
        if (!m_inputHandler->didNodeOpenPopup(node))
            eventHandler->handleMouseReleaseEvent(mouseEvent);
    }

    return true;
}
