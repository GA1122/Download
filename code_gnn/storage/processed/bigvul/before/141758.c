void V8Debugger::unmuteScriptParsedEvents()
{
    --m_ignoreScriptParsedEventsCounter;
    DCHECK_GE(m_ignoreScriptParsedEventsCounter, 0);
}
