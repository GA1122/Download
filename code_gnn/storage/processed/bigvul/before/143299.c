void Document::updateViewportDescription()
{
    if (frame() && frame()->isMainFrame()) {
        frameHost()->chromeClient().dispatchViewportPropertiesDidChange(viewportDescription());
    }
}
