bool InputHandler::setBatchEditingActive(bool active)
{
    if (!isActiveTextEdit())
        return false;

    ASSERT(m_currentFocusElement->document());
    ASSERT(m_currentFocusElement->document()->frame());

    BackingStoreClient* backingStoreClient = m_webPage->backingStoreClient();
    ASSERT(backingStoreClient);

    if (!active) {
        backingStoreClient->backingStore()->resumeBackingStoreUpdates();
        backingStoreClient->backingStore()->resumeScreenUpdates(BackingStore::RenderAndBlit);
    } else {
        backingStoreClient->backingStore()->suspendBackingStoreUpdates();
        backingStoreClient->backingStore()->suspendScreenUpdates();
    }

    return true;
}
