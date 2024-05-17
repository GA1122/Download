void HTMLDocumentParser::executeScriptsWaitingForStylesheets()
{
    ASSERT(m_scriptRunner);
    if (!m_scriptRunner->hasScriptsWaitingForStylesheets())
        return;

    RefPtr<HTMLDocumentParser> protect(this);
    m_scriptRunner->executeScriptsWaitingForStylesheets();
    if (!isWaitingForScripts())
        resumeParsingAfterScriptExecution();
}
