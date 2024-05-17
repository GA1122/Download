CustomElementMicrotaskRunQueue* Document::customElementMicrotaskRunQueue()
{
    if (!m_customElementMicrotaskRunQueue)
        m_customElementMicrotaskRunQueue = CustomElementMicrotaskRunQueue::create();
    return m_customElementMicrotaskRunQueue.get();
}
