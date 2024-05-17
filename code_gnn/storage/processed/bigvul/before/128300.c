void FrameView::prepareForDetach()
{
    RELEASE_ASSERT(!isInPerformLayout());

    if (ScrollAnimator* scrollAnimator = existingScrollAnimator())
        scrollAnimator->cancelAnimations();
    cancelProgrammaticScrollAnimation();

    detachCustomScrollbars();
    removeFromAXObjectCache();

    if (m_frame->page()) {
        if (ScrollingCoordinator* scrollingCoordinator = m_frame->page()->scrollingCoordinator())
            scrollingCoordinator->willDestroyScrollableArea(this);
    }
}
