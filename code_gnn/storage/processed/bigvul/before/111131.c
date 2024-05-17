void WebPagePrivate::dispatchViewportPropertiesDidChange(const ViewportArguments& arguments)
{
    if (arguments == m_viewportArguments)
        return;

    static const ViewportArguments defaultViewportArguments;
    if (arguments == defaultViewportArguments) {
        m_viewportArguments = m_userViewportArguments;
        m_forceRespectViewportArguments = m_userViewportArguments != defaultViewportArguments;
    } else
        m_viewportArguments = arguments;

    if (!m_viewportArguments.width)
        m_viewportArguments.width = ViewportArguments::ValueDeviceWidth;
    if (!m_viewportArguments.height)
        m_viewportArguments.height = ViewportArguments::ValueDeviceHeight;

    Platform::IntSize virtualViewport = recomputeVirtualViewportFromViewportArguments();
    m_webPage->setVirtualViewportSize(virtualViewport);

    bool isLoadFinished = loadState() == Finished;
    if (isLoadFinished) {
        m_userPerformedManualZoom = false;
        setShouldZoomToInitialScaleAfterLoadFinished(true);
    }
    if (loadState() == Committed || isLoadFinished)
        zoomToInitialScaleOnLoad();
}
