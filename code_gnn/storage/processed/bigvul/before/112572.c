void Document::setVisualUpdatesAllowed(bool visualUpdatesAllowed)
{
    if (m_visualUpdatesAllowed == visualUpdatesAllowed)
        return;

    m_visualUpdatesAllowed = visualUpdatesAllowed;

    if (!visualUpdatesAllowed)
        return;

    FrameView* frameView = view();
    bool needsLayout = frameView && renderer() && (frameView->layoutPending() || renderer()->needsLayout());
    if (needsLayout) {
        return;
    }

#if USE(ACCELERATED_COMPOSITING)
    if (view())
        view()->updateCompositingLayersAfterLayout();
#endif

    if (renderer())
        renderer()->repaint();
}
