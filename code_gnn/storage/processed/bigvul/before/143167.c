V0CustomElementMicrotaskRunQueue* Document::customElementMicrotaskRunQueue()
{
    if (!m_customElementMicrotaskRunQueue)
        m_customElementMicrotaskRunQueue = V0CustomElementMicrotaskRunQueue::create();
    return m_customElementMicrotaskRunQueue.get();
}
