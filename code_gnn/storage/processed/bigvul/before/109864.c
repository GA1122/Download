void Document::updateStyleIfNeeded()
{
    ASSERT(isMainThread());
    ASSERT(!view() || (!view()->isInLayout() && !view()->isPainting()));

    if (!needsStyleRecalc() && !childNeedsStyleRecalc() && !childNeedsDistributionRecalc())
        return;

    AnimationUpdateBlock animationUpdateBlock(m_frame ? m_frame->animation() : 0);
    recalcStyle(NoChange);
}
