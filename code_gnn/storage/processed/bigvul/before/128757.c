ReadableStreamReader* ReadableStream::getReader(ExecutionContext* executionContext, ExceptionState& exceptionState)
{
    if (m_reader) {
        exceptionState.throwTypeError("already locked to a ReadableStreamReader");
        return nullptr;
    }
    return new ReadableStreamReader(executionContext, this);
}
