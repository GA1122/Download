void ReadableStream::error(DOMException* exception)
{
    if (m_state != ReadableStream::Readable)
        return;

    m_exception = exception;
    clearQueue();
    rejectAllPendingReads(m_exception);
    m_state = Errored;
    if (m_reader)
        m_reader->error();
}
