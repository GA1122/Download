bool FrameView::isRubberBandInProgress() const
{
    if (scrollbarsSuppressed())
        return false;

    if (ScrollAnimator* scrollAnimator = existingScrollAnimator())
        return scrollAnimator->isRubberBandInProgress();

    return false;
}
