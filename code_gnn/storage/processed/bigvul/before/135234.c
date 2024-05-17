ScriptedIdleTaskController& Document::ensureScriptedIdleTaskController()
{
    if (!m_scriptedIdleTaskController)
        m_scriptedIdleTaskController = ScriptedIdleTaskController::create(this);
    return *m_scriptedIdleTaskController;
}
