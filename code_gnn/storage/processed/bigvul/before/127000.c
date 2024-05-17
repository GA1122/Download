ChromeClient::CompositingTriggerFlags ChromeClientImpl::allowedCompositingTriggers() const
{
    if (!m_webView->allowsAcceleratedCompositing())
        return 0;

    CompositingTriggerFlags flags = 0;
    Settings& settings = m_webView->page()->settings();
    if (settings.acceleratedCompositingFor3DTransformsEnabled())
        flags |= ThreeDTransformTrigger;
    if (settings.acceleratedCompositingForVideoEnabled())
        flags |= VideoTrigger;
    if (settings.acceleratedCompositingForPluginsEnabled())
        flags |= PluginTrigger;
    if (settings.acceleratedCompositingForAnimationEnabled())
        flags |= AnimationTrigger;
    if (settings.acceleratedCompositingForCanvasEnabled())
        flags |= CanvasTrigger;
    if (settings.acceleratedCompositingForScrollableFramesEnabled())
        flags |= ScrollableInnerFrameTrigger;
    if (settings.acceleratedCompositingForFiltersEnabled())
        flags |= FilterTrigger;

    return flags;
}
