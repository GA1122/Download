bool ScriptController::haveInterpreter() const
{
    return m_proxy->windowShell()->isContextInitialized();
}
