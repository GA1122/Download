void FrameView::updateLayersAndCompositingAfterScrollIfNeeded()
{
    if (!hasViewportConstrainedObjects())
        return;

    RefPtr<FrameView> protect(this);

    if (!m_nestedLayoutCount) {
        updateWidgetPositions();
        if (RenderView* renderView = this->renderView())
            renderView->layer()->setNeedsCompositingInputsUpdate();
    }
}
