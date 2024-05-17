void Document::unscheduleStyleRecalc()
{
    ASSERT(!confusingAndOftenMisusedAttached() || (!needsStyleRecalc() && !childNeedsStyleRecalc()));
    m_styleRecalcTimer.stop();
}
