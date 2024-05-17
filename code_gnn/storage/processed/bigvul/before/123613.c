bool InspectorPageAgent::forceCompositingMode(ErrorString* errorString)
{
    Settings& settings = m_page->settings();
    if (!settings.acceleratedCompositingEnabled()) {
        if (errorString)
            *errorString = "Compositing mode is not supported";
        return false;
    }
    if (settings.forceCompositingMode())
        return true;
    settings.setForceCompositingMode(true);
    Frame* mainFrame = m_page->mainFrame();
    if (mainFrame)
        mainFrame->view()->updateCompositingLayersAfterStyleChange();
    return true;
}
