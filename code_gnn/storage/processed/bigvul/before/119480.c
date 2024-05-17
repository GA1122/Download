ScriptValue ScriptController::executeScript(const ScriptSourceCode& sourceCode)
{
    if (!canExecuteScripts(AboutToExecuteScript) || isPaused())
        return ScriptValue();

    RefPtr<Frame> protect(m_frame);  

    return executeScriptInMainWorld(sourceCode);
}
