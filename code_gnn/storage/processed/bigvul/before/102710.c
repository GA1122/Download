void CCThreadProxy::drawLayersAndReadbackOnCCThread(CCCompletionEvent* completion, bool* success, void* pixels, const IntRect& rect)
{
    ASSERT(CCProxy::isImplThread());
    if (!m_layerTreeHostImpl) {
        *success = false;
        completion->signal();
        return;
    }
    drawLayersOnCCThread();
    m_layerTreeHostImpl->readback(pixels, rect);
    *success = m_layerTreeHostImpl->isContextLost();
    completion->signal();
}
