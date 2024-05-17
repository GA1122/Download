void InspectorPageAgent::setForceCompositingMode(ErrorString* errorString, bool force)
{
    Settings& settings = m_page->settings();
    if (force && !settings.acceleratedCompositingEnabled()) {
        if (errorString)
            *errorString = "Compositing mode is not supported";
        return;
    }
    m_state->setBoolean(PageAgentState::forceCompositingMode, force);
    if (settings.forceCompositingMode() == force)
        return;
    m_didForceCompositingMode = force;
    settings.setForceCompositingMode(force);
    Frame* mainFrame = m_page->mainFrame();
    if (!mainFrame)
        return;
    mainFrame->view()->updateCompositingLayersAfterStyleChange();
}
