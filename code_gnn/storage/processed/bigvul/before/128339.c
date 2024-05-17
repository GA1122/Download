void FrameView::setFrameRect(const IntRect& newRect)
{
    IntRect oldRect = frameRect();
    if (newRect == oldRect)
        return;

    bool autosizerNeedsUpdating = false;
    if (newRect.width() != oldRect.width() && m_frame->isMainFrame() && m_frame->settings()->textAutosizingEnabled())
        autosizerNeedsUpdating = true;

    ScrollView::setFrameRect(newRect);

    updateScrollableAreaSet();

    if (autosizerNeedsUpdating) {
        if (TextAutosizer* textAutosizer = m_frame->document()->textAutosizer())
            textAutosizer->updatePageInfoInAllFrames();
    }

    if (RenderView* renderView = this->renderView()) {
        if (renderView->usesCompositing())
            renderView->compositor()->frameViewDidChangeSize();
    }

    viewportConstrainedVisibleContentSizeChanged(newRect.width() != oldRect.width(), newRect.height() != oldRect.height());

    if (oldRect.size() != newRect.size()
        && m_frame->isMainFrame()
        && m_frame->settings()->pinchVirtualViewportEnabled())
        page()->frameHost().pinchViewport().mainFrameDidChangeSize();
}
