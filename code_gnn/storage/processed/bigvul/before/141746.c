void V8Debugger::muteScriptParsedEvents()
{
    ++m_ignoreScriptParsedEventsCounter;
}
