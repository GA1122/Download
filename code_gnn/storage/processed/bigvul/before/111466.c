void InputHandler::ensureFocusTextElementVisible(CaretScrollType scrollType)
{
    if (!isActiveTextEdit() || !isInputModeEnabled() || !m_currentFocusElement->document())
        return;

    if (!(Platform::Settings::instance()->allowedScrollAdjustmentForInputFields() & scrollType))
        return;

    if (DOMSupport::isFixedPositionOrHasFixedPositionAncestor(m_currentFocusElement->renderer()))
        return;

    Frame* elementFrame = m_currentFocusElement->document()->frame();
    if (!elementFrame)
        return;

    Frame* mainFrame = m_webPage->mainFrame();
    if (!mainFrame)
        return;

    FrameView* mainFrameView = mainFrame->view();
    if (!mainFrameView)
        return;

    WebCore::IntRect selectionFocusRect;
    switch (elementFrame->selection()->selectionType()) {
    case VisibleSelection::CaretSelection:
        selectionFocusRect = elementFrame->selection()->absoluteCaretBounds();
        break;
    case VisibleSelection::RangeSelection: {
        Position selectionPosition;
        if (m_webPage->m_selectionHandler->lastUpdatedEndPointIsValid())
            selectionPosition = elementFrame->selection()->end();
        else
            selectionPosition = elementFrame->selection()->start();
        selectionFocusRect = VisiblePosition(selectionPosition).absoluteCaretBounds();
        break;
    }
    case VisibleSelection::NoSelection:
        m_shouldEnsureFocusTextElementVisibleOnSelectionChanged = true;
        return;
    }

    int fontHeight = selectionFocusRect.height();

    static const int s_minimumTextHeightInPixels = Graphics::Screen::primaryScreen()->heightInMMToPixels(3);

    double zoomScaleRequired;
    if (m_webPage->isUserScalable() && fontHeight && fontHeight * m_webPage->currentScale() < s_minimumTextHeightInPixels && !isRunningDrt())
        zoomScaleRequired = static_cast<double>(s_minimumTextHeightInPixels) / fontHeight;
    else
        zoomScaleRequired = m_webPage->currentScale();  

    WebCore::FloatPoint offset(selectionFocusRect.location().x() - m_webPage->scrollPosition().x(), selectionFocusRect.location().y() - m_webPage->scrollPosition().y());
    double inverseScale = zoomScaleRequired / m_webPage->currentScale();
    WebCore::IntPoint destinationScrollLocation = WebCore::IntPoint(max(0, static_cast<int>(roundf(selectionFocusRect.location().x() - offset.x() / inverseScale))),
                                                                    max(0, static_cast<int>(roundf(selectionFocusRect.location().y() - offset.y() / inverseScale))));

    if (elementFrame != mainFrame) {  
        selectionFocusRect.move(-elementFrame->view()->scrollPosition().x(), -elementFrame->view()->scrollPosition().y());

        if (elementFrame->ownerRenderer()) {
            WebCore::IntPoint frameOffset = elementFrame->ownerRenderer()->absoluteContentBox().location();
            selectionFocusRect.move(frameOffset.x(), frameOffset.y());
        }
    }

    bool shouldConstrainScrollingToContentEdge = true;
    Position start = elementFrame->selection()->start();
    if (start.anchorNode() && start.anchorNode()->renderer()) {
        if (RenderLayer* layer = start.anchorNode()->renderer()->enclosingLayer()) {
            WebCore::IntRect actualScreenRect = WebCore::IntRect(destinationScrollLocation.x(), destinationScrollLocation.y(), m_webPage->actualVisibleSize().width() / inverseScale, m_webPage->actualVisibleSize().height() / inverseScale);

            ScrollAlignment horizontalScrollAlignment = ScrollAlignment::alignToEdgeIfNeeded;
            ScrollAlignment verticalScrollAlignment = ScrollAlignment::alignToEdgeIfNeeded;

            if (scrollType != EdgeIfNeeded) {
                if (RenderObject* focusedRenderer = m_currentFocusElement->renderer()) {
                    WebCore::IntRect nodeOutlineBounds = focusedRenderer->absoluteOutlineBounds();
                    WebCore::IntRect caretAtEdgeRect = rectForCaret(0);
                    int paddingX = abs(caretAtEdgeRect.x() - nodeOutlineBounds.x());
                    int paddingY = abs(caretAtEdgeRect.y() - nodeOutlineBounds.y());

                    if (selectionFocusRect.x() - paddingX == nodeOutlineBounds.x())
                        selectionFocusRect.setX(nodeOutlineBounds.x());
                    else if (selectionFocusRect.maxX() + paddingX == nodeOutlineBounds.maxX())
                        selectionFocusRect.setX(nodeOutlineBounds.maxX() - selectionFocusRect.width());
                    if (selectionFocusRect.y() - paddingY == nodeOutlineBounds.y())
                        selectionFocusRect.setY(nodeOutlineBounds.y() - selectionFocusRect.height());
                    else if (selectionFocusRect.maxY() + paddingY == nodeOutlineBounds.maxY())
                        selectionFocusRect.setY(nodeOutlineBounds.maxY() - selectionFocusRect.height());

                    if (selectionFocusRect.x() - caretAtEdgeRect.x() < actualScreenRect.width() / 2)
                        selectionFocusRect.setX(nodeOutlineBounds.x());
                    else
                        horizontalScrollAlignment = ScrollAlignment::alignCenterIfNeeded;

                }
                verticalScrollAlignment = (scrollType == CenterAlways) ? ScrollAlignment::alignCenterAlways : ScrollAlignment::alignCenterIfNeeded;
            }

            static const int s_focusRectPaddingSize = Graphics::Screen::primaryScreen()->heightInMMToPixels(3);
            selectionFocusRect.inflate(m_webPage->mapFromTransformed(WebCore::IntSize(0, s_focusRectPaddingSize)).height());

            WebCore::IntRect revealRect(layer->getRectToExpose(actualScreenRect, selectionFocusRect,
                                                                 horizontalScrollAlignment,
                                                                 verticalScrollAlignment));

            shouldConstrainScrollingToContentEdge = false;

            destinationScrollLocation = revealRect.location();
            destinationScrollLocation.clampNegativeToZero();
            WebCore::IntPoint maximumScrollPosition = WebCore::IntPoint(mainFrameView->contentsWidth() - actualScreenRect.width(), mainFrameView->contentsHeight() - actualScreenRect.height());
            destinationScrollLocation = destinationScrollLocation.shrunkTo(maximumScrollPosition);
        }
    }

    if (destinationScrollLocation != mainFrameView->scrollPosition() || zoomScaleRequired != m_webPage->currentScale()) {
        InputLog(LogLevelInfo, "InputHandler::ensureFocusTextElementVisible zooming in to %f and scrolling to point %d, %d", zoomScaleRequired, destinationScrollLocation.x(), destinationScrollLocation.y());

        m_webPage->m_finalBlockPoint = WebCore::FloatPoint(destinationScrollLocation);
        m_webPage->m_blockZoomFinalScale = zoomScaleRequired;
        m_webPage->m_shouldReflowBlock = false;
        m_webPage->m_userPerformedManualZoom = true;
        m_webPage->m_userPerformedManualScroll = true;
        m_webPage->m_shouldConstrainScrollingToContentEdge = shouldConstrainScrollingToContentEdge;
        m_webPage->client()->animateBlockZoom(zoomScaleRequired, m_webPage->m_finalBlockPoint);
    }
}
