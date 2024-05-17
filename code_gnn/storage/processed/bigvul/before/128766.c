ScriptPromise ReadableStreamReader::closed(ScriptState* scriptState)
{
    return m_closed->promise(scriptState->world());
}
