PassRefPtr<Node> WebPagePrivate::contextNode(TargetDetectionStrategy strategy)
{
    EventHandler* eventHandler = focusedOrMainFrame()->eventHandler();
    const FatFingersResult lastFatFingersResult = m_touchEventHandler->lastFatFingersResult();
    bool isTouching = lastFatFingersResult.isValid() && strategy == RectBased;

    if (m_webSettings->doesGetFocusNodeContext() && !isTouching) {
        RefPtr<Node> node;
        node = m_page->focusController()->focusedOrMainFrame()->document()->focusedNode();
        if (node) {
            IntRect visibleRect = IntRect(IntPoint(), actualVisibleSize());
            if (!visibleRect.intersects(getNodeWindowRect(node.get())))
                return 0;
        }
        return node.release();
    }

    if (isTouching && lastFatFingersResult.isTextInput())
        return lastFatFingersResult.node(FatFingersResult::ShadowContentNotAllowed);

    if (strategy == RectBased) {
        FatFingersResult result = FatFingers(this, lastFatFingersResult.adjustedPosition(), FatFingers::Text).findBestPoint();
        return result.node(FatFingersResult::ShadowContentNotAllowed);
    }
    if (strategy == FocusBased)
        return m_inputHandler->currentFocusElement();

    IntPoint contentPos;
    if (isTouching)
        contentPos = lastFatFingersResult.adjustedPosition();
    else
        contentPos = mapFromViewportToContents(m_lastMouseEvent.position());

    HitTestResult result = eventHandler->hitTestResultAtPoint(contentPos);
    return result.innerNode();
}
