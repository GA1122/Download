ReadableStreamReader::ReadableStreamReader(ExecutionContext* executionContext, ReadableStream* stream)
    : ActiveScriptWrappable(this)
    , ActiveDOMObject(executionContext)
    , m_stream(stream)
    , m_closed(new ClosedPromise(executionContext, this, ClosedPromise::Closed))
{
    suspendIfNeeded();
    ASSERT(m_stream->isLockedTo(nullptr));
    m_stream->setReader(this);

    if (m_stream->stateInternal() == ReadableStream::Closed)
        m_closed->resolve(ToV8UndefinedGenerator());
    if (m_stream->stateInternal() == ReadableStream::Errored)
        m_closed->reject(m_stream->storedException());
}
