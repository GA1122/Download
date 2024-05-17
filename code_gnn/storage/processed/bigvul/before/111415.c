void WebPagePrivate::updateDelegatedOverlays(bool dispatched)
{
    if (dispatched)
        m_updateDelegatedOverlaysDispatched = false;
    else if (m_updateDelegatedOverlaysDispatched) {
        return;
    }

    if (Platform::webKitThreadMessageClient()->isCurrentThread()) {
        if (m_selectionHandler->isSelectionActive())
            m_selectionHandler->selectionPositionChanged();
        if (m_inspectorOverlay)
            m_inspectorOverlay->update();

    } else if (m_selectionHandler->isSelectionActive()) {
        m_updateDelegatedOverlaysDispatched = true;
        Platform::webKitThreadMessageClient()->dispatchMessage(Platform::createMethodCallMessage(&WebPagePrivate::updateDelegatedOverlays, this, true  ));
    }
}
