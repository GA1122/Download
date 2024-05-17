void ReadableStream::close()
{
    if (m_state != Readable)
        return;

    if (isQueueEmpty())
        closeInternal();
    else
        m_isDraining = true;
}
