void WebPagePrivate::setViewportSize(const IntSize& transformedActualVisibleSize, bool ensureFocusElementVisible)
{
    if (m_pendingOrientation == -1 && transformedActualVisibleSize == this->transformedActualVisibleSize())
        return;

    BackingStore::ResumeUpdateOperation screenResumeOperation = BackingStore::Blit;
    m_backingStore->d->suspendScreenUpdates();
    m_backingStore->d->suspendBackingStoreUpdates();

    if (!m_visible || !m_backingStore->d->isActive())
        setShouldResetTilesWhenShown(true);

    bool hasPendingOrientation = m_pendingOrientation != -1;

    IntSize viewportSizeBefore = actualVisibleSize();
    FloatPoint centerOfVisibleContentsRect = this->centerOfVisibleContentsRect();
    bool newVisibleRectContainsOldVisibleRect = (m_actualVisibleHeight <= transformedActualVisibleSize.height())
                                          && (m_actualVisibleWidth <= transformedActualVisibleSize.width());

    bool atInitialScale = m_webPage->isAtInitialZoom();
    bool atTop = !scrollPosition().y();
    bool atLeft = !scrollPosition().x();

    setDefaultLayoutSize(transformedActualVisibleSize);

    bool needsLayout = false;
    static ViewportArguments defaultViewportArguments;
    if (m_viewportArguments != defaultViewportArguments) {
        Platform::IntSize newVirtualViewport = recomputeVirtualViewportFromViewportArguments();
        ASSERT(!newVirtualViewport.isEmpty());
        m_webPage->setVirtualViewportSize(newVirtualViewport);
        m_mainFrame->view()->setUseFixedLayout(useFixedLayout());
        m_mainFrame->view()->setFixedLayoutSize(fixedLayoutSize());
        needsLayout = true;
    }

    m_actualVisibleWidth = transformedActualVisibleSize.width();
    m_actualVisibleHeight = transformedActualVisibleSize.height();

    IntSize viewportSizeAfter = actualVisibleSize();

    IntSize offset;
    if (hasPendingOrientation && !m_fullscreenVideoNode) {
        offset = IntSize(roundf((viewportSizeBefore.width() - viewportSizeAfter.width()) / 2.0),
            roundf((viewportSizeBefore.height() - viewportSizeAfter.height()) / 2.0));
    }

    if (atTop)
        offset.setHeight(0);
    if (atLeft)
        offset.setWidth(0);

    IntPoint bottomRight(
        scrollPosition().x() + viewportSizeAfter.width(),
        scrollPosition().y() + viewportSizeAfter.height());

    if (bottomRight.x() + offset.width() > contentsSize().width())
        offset.setWidth(contentsSize().width() - bottomRight.x());
    if (bottomRight.y() + offset.height() > contentsSize().height())
        offset.setHeight(contentsSize().height() - bottomRight.y());
    if (scrollPosition().x() + offset.width() < 0)
        offset.setWidth(-scrollPosition().x());
    if (scrollPosition().y() + offset.height() < 0)
        offset.setHeight(-scrollPosition().y());

    setScrollPosition(scrollPosition() + offset);
    notifyTransformedScrollChanged();

    m_backingStore->d->orientationChanged();
    m_backingStore->d->actualVisibleSizeChanged(transformedActualVisibleSize);

    if (setViewMode(viewMode()))
        needsLayout = true;

    bool needsLayoutToFindContentSize = hasPendingOrientation;

    updateViewportSize(!needsLayoutToFindContentSize  , false  );
    notifyTransformedContentsSizeChanged();

    if (!m_webSettings->isZoomToFitOnLoad()) {
        atInitialScale = false;

        if (!hasVirtualViewport() && contentsSize().width() < m_defaultLayoutSize.width()) {
            m_mainFrame->view()->setUseFixedLayout(useFixedLayout());
            m_mainFrame->view()->setFixedLayoutSize(m_defaultLayoutSize);
            needsLayout = true;
        }
    }

    m_backingStore->d->resumeBackingStoreUpdates();

    bool stillNeedsLayout = needsLayout;
    while (stillNeedsLayout) {
        setNeedsLayout();
        requestLayoutIfNeeded();
        stillNeedsLayout = false;

        ++m_nestedLayoutFinishedCount;
        if (needsLayoutToFindContentSize) {
            if (setViewMode(viewMode()))
                stillNeedsLayout = true;
        }
    }
    m_nestedLayoutFinishedCount = 0;

    double scale = atInitialScale ? initialScale() : currentScale();

    scale = clampedScale(scale);

    if (needsLayoutToFindContentSize) {
        TransformationMatrix rotationMatrix;
        rotationMatrix.scale(scale);
        IntRect viewportRect = IntRect(IntPoint::zero(), transformedActualVisibleSize);
        IntRect actualVisibleRect = enclosingIntRect(rotationMatrix.inverse().mapRect(FloatRect(viewportRect)));
        m_mainFrame->view()->setFixedReportedSize(actualVisibleRect.size());
        m_mainFrame->view()->repaintFixedElementsAfterScrolling();
        requestLayoutIfNeeded();
        m_mainFrame->view()->updateFixedElementsAfterScrolling();
    }

    if (shouldSendResizeEvent())
        m_mainFrame->eventHandler()->sendResizeEvent();

    FloatPoint anchor = centerOfVisibleContentsRect;
    if (atTop)
        anchor.setY(0);
    if (atLeft)
        anchor.setX(0);

    if (zoomAboutPoint(scale, anchor, false  , true  )) {
        if (ensureFocusElementVisible)
            ensureContentVisible(!newVisibleRectContainsOldVisibleRect);
    } else {
        m_backingStore->d->suspendBackingStoreUpdates();

        IntPoint scrollPosition = this->scrollPosition();
        if (atTop)
            scrollPosition.setY(0);
        if (atLeft)
            scrollPosition.setX(0);
        setScrollPosition(scrollPosition);

        notifyTransformedContentsSizeChanged();
        notifyTransformedScrollChanged();

        if (!needsLayout) {
            m_backingStore->d->updateTilesForScrollOrNotRenderedRegion();
        }

        if (ensureFocusElementVisible)
            ensureContentVisible(!newVisibleRectContainsOldVisibleRect);

        if (needsLayout) {
            m_backingStore->d->resetTiles();
            m_backingStore->d->updateTiles(false  , false  );
            screenResumeOperation = BackingStore::RenderAndBlit;
        }

        m_backingStore->d->resumeBackingStoreUpdates();
    }

#if ENABLE(FULLSCREEN_API) && ENABLE(VIDEO)
    if (m_scaleBeforeFullScreen > 0 && !m_fullscreenNode) {
        if (m_orientationBeforeFullScreen % 180 != orientation() % 180) {  
            if (m_actualVisibleWidth > contentsSize().width() * m_scaleBeforeFullScreen) {
                m_scaleBeforeFullScreen = double(m_actualVisibleWidth) / contentsSize().width();
            }
            if (m_scaleBeforeFullScreen * contentsSize().height() < m_actualVisibleHeight) {
                m_scaleBeforeFullScreen = double(m_actualVisibleHeight) / contentsSize().height();
            }

            if (m_actualVisibleWidth > m_scaleBeforeFullScreen * (contentsSize().width() - m_scrollPositionBeforeFullScreen.x())) {
                m_scrollPositionBeforeFullScreen.setX(contentsSize().width() - m_actualVisibleWidth / m_scaleBeforeFullScreen);
            }
            if (m_actualVisibleHeight > m_scaleBeforeFullScreen * (contentsSize().height() - m_scrollPositionBeforeFullScreen.y())) {
                m_scrollPositionBeforeFullScreen.setY(contentsSize().height() - m_actualVisibleHeight / m_scaleBeforeFullScreen);
            }
        }

        m_transformationMatrix->setM11(m_scaleBeforeFullScreen);
        m_transformationMatrix->setM22(m_scaleBeforeFullScreen);
        m_scaleBeforeFullScreen = -1.0;

        setScrollPosition(m_scrollPositionBeforeFullScreen);
        notifyTransformChanged();
        m_client->scaleChanged();
    }
#endif

    m_backingStore->d->resumeScreenUpdates(screenResumeOperation);
    m_inputHandler->redrawSpellCheckDialogIfRequired();
}
