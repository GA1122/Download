void WebPagePrivate::zoomToInitialScaleOnLoad()
{
#if DEBUG_WEBPAGE_LOAD
    Platform::logAlways(Platform::LogLevelInfo, "WebPagePrivate::zoomToInitialScaleOnLoad");
#endif

    bool needsLayout = false;

    if (m_shouldUseFixedDesktopMode)
        needsLayout = setViewMode(FixedDesktop);
    else
        needsLayout = setViewMode(Desktop);

    if (needsLayout) {
        setNeedsLayout();
    }

    if (contentsSize().isEmpty()) {
#if DEBUG_WEBPAGE_LOAD
        Platform::logAlways(Platform::LogLevelInfo, "WebPagePrivate::zoomToInitialScaleOnLoad content is empty!");
#endif
        requestLayoutIfNeeded();
        notifyTransformedContentsSizeChanged();
        return;
    }

    bool performedZoom = false;
    bool shouldZoom = !m_userPerformedManualZoom;

    if (m_mainFrame && m_mainFrame->loader() && isBackForwardLoadType(m_mainFrame->loader()->loadType()))
        shouldZoom = false;

    if (shouldZoom && shouldZoomToInitialScaleOnLoad()) {
        FloatPoint anchor = centerOfVisibleContentsRect();
        if (!scrollPosition().x())
            anchor.setX(0);
        if (!scrollPosition().y())
            anchor.setY(0);
        performedZoom = zoomAboutPoint(initialScale(), anchor);
    }

    requestLayoutIfNeeded();

    if (!performedZoom) {
        notifyTransformedContentsSizeChanged();
    }
}
