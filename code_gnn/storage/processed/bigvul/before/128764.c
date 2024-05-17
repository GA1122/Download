ScriptPromise ReadableStreamReader::cancel(ScriptState* scriptState, ScriptValue reason)
{
    if (isActive())
        return m_stream->cancelInternal(scriptState, reason);

    return ScriptPromise::reject(scriptState, V8ThrowException::createTypeError(scriptState->isolate(), "the reader is already released"));
}
