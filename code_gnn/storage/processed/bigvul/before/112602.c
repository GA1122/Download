void Document::unscheduleStyleRecalc()
{
    ASSERT(!childNeedsStyleRecalc());

    if (documentsThatNeedStyleRecalc)
        documentsThatNeedStyleRecalc->remove(this);

    m_styleRecalcTimer.stop();
    m_pendingStyleRecalcShouldForce = false;
}
