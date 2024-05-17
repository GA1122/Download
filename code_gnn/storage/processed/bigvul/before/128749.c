ScriptPromise ReadableStream::cancel(ScriptState* scriptState, ScriptValue reason)
{
    if (m_reader)
        return ScriptPromise::reject(scriptState, V8ThrowException::createTypeError(scriptState->isolate(), "this stream is locked to a ReadableStreamReader"));
    setIsDisturbed();
    if (m_state == Closed)
        return ScriptPromise::castUndefined(scriptState);
    if (m_state == Errored)
        return ScriptPromise::rejectWithDOMException(scriptState, m_exception);

    return cancelInternal(scriptState, reason);
}
