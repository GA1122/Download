void InspectorPageAgent::scriptsEnabled(bool isEnabled)
{
    if (m_ignoreScriptsEnabledNotification)
        return;

    m_frontend->scriptsEnabled(isEnabled);
}
