ScriptPromise ReadableStreamReader::read(ScriptState* scriptState)
{
    if (!isActive())
        return ScriptPromise::reject(scriptState, V8ThrowException::createTypeError(scriptState->isolate(), "the reader is already released"));

    return m_stream->read(scriptState);
}
