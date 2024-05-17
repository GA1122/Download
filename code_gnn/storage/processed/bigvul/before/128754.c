bool ReadableStream::enqueuePostAction()
{
    m_isPulling = false;

    bool shouldApplyBackpressure = this->shouldApplyBackpressure();
    if (m_state == Errored)
        return false;

    return !shouldApplyBackpressure;
}
