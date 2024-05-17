void Document::updateViewportDescription()
{
    if (frame() && frame()->isMainFrame()) {
        frameHost()->chrome().dispatchViewportPropertiesDidChange(m_viewportDescription);
    }
}
