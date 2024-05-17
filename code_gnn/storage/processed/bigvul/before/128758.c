void ReadableStream::readInternalPostAction()
{
    ASSERT(m_state == Readable);
    if (isQueueEmpty() && m_isDraining)
        closeInternal();
    callPullIfNeeded();
}
