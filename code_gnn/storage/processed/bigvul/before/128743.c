ReadableByteStreamReader* ReadableByteStream::getBytesReader(ExecutionContext* executionContext, ExceptionState& es)
{
    ReadableStreamReader* reader = getReader(executionContext, es);
    if (es.hadException())
        return nullptr;
    return new ReadableByteStreamReader(reader);
}
