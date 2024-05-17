void FrameView::reset()
{
    m_hasPendingLayout = false;
    m_layoutSubtreeRoot = 0;
    m_doFullPaintInvalidation = false;
    m_layoutSchedulingEnabled = true;
    m_inPerformLayout = false;
    m_canInvalidatePaintDuringPerformLayout = false;
    m_inSynchronousPostLayout = false;
    m_layoutCount = 0;
    m_nestedLayoutCount = 0;
    m_postLayoutTasksTimer.stop();
    m_updateWidgetsTimer.stop();
    m_firstLayout = true;
    m_firstLayoutCallbackPending = false;
    m_wasScrolledByUser = false;
    m_safeToPropagateScrollToParent = true;
    m_lastViewportSize = IntSize();
    m_lastZoomFactor = 1.0f;
    m_isTrackingPaintInvalidations = false;
    m_trackedPaintInvalidationRects.clear();
    m_lastPaintTime = 0;
    m_paintBehavior = PaintBehaviorNormal;
    m_isPainting = false;
    m_visuallyNonEmptyCharacterCount = 0;
    m_visuallyNonEmptyPixelCount = 0;
    m_isVisuallyNonEmpty = false;
    m_firstVisuallyNonEmptyLayoutCallbackPending = true;
    m_maintainScrollPositionAnchor = nullptr;
    m_viewportConstrainedObjects.clear();
}
