bool WebPagePrivate::zoomAboutPoint(double unclampedScale, const FloatPoint& anchor, bool enforceScaleClamping, bool forceRendering, bool isRestoringZoomLevel)
{
    if (!isRestoringZoomLevel) {
        resetBlockZoom();
    }

#if ENABLE(VIEWPORT_REFLOW)
    toggleTextReflowIfEnabledForBlockZoomOnly(m_shouldReflowBlock);
    if (m_page->settings()->isTextReflowEnabled() && m_mainFrame->view())
        setNeedsLayout();
#endif

    double scale;
    if (!shouldZoomAboutPoint(unclampedScale, anchor, enforceScaleClamping, &scale)) {
        if (m_webPage->settings()->textReflowMode() == WebSettings::TextReflowEnabled) {
            m_currentPinchZoomNode = 0;
            m_anchorInNodeRectRatio = FloatPoint(-1, -1);
        }
        return false;
    }
    TransformationMatrix zoom;
    zoom.scale(scale);

#if DEBUG_WEBPAGE_LOAD
    if (loadState() < Finished) {
        Platform::logAlways(Platform::LogLevelInfo,
            "WebPagePrivate::zoomAboutPoint scale %f anchor %s",
            scale, Platform::FloatPoint(anchor).toString().c_str());
    }
#endif

    FloatPoint scrollPosition = this->scrollPosition();

    FloatPoint anchorOffset(anchor.x() - scrollPosition.x(), anchor.y() - scrollPosition.y());

    ASSERT(m_transformationMatrix->m11() == m_transformationMatrix->m22());

    double inverseScale = scale / m_transformationMatrix->m11();

    *m_transformationMatrix = zoom;

    m_backingStore->d->suspendBackingStoreUpdates();
    m_backingStore->d->suspendScreenUpdates();

    updateViewportSize();

    IntPoint newScrollPosition(IntPoint(max(0, static_cast<int>(roundf(anchor.x() - anchorOffset.x() / inverseScale))),
                                        max(0, static_cast<int>(roundf(anchor.y() - anchorOffset.y() / inverseScale)))));

    if (m_webPage->settings()->textReflowMode() == WebSettings::TextReflowEnabled) {
        m_mainFrame->view()->setNeedsLayout();
        requestLayoutIfNeeded();
        if (m_currentPinchZoomNode)
            newScrollPosition = calculateReflowedScrollPosition(anchorOffset, scale == minimumScale() ? 1 : inverseScale);
         m_currentPinchZoomNode = 0;
         m_anchorInNodeRectRatio = FloatPoint(-1, -1);
    }

    setScrollPosition(newScrollPosition);

    notifyTransformChanged();

    bool isLoading = this->isLoading();

    m_backingStore->d->updateTiles(isLoading  , false  );

    bool shouldRender = !isLoading || m_userPerformedManualZoom || forceRendering;

    m_client->scaleChanged();

    if (m_pendingOrientation != -1)
        m_client->updateInteractionViews();

    m_backingStore->d->resumeBackingStoreUpdates();

    if (shouldRender) {
        m_backingStore->d->resumeScreenUpdates(BackingStore::RenderAndBlit);
    } else {
        m_backingStore->d->resumeScreenUpdates(BackingStore::None);
    }

    return true;
}
