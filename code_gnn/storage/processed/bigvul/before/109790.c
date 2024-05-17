void Document::scheduleStyleRecalc()
{
    if (shouldDisplaySeamlesslyWithParent()) {
        ownerElement()->setNeedsStyleRecalc();
        ownerElement()->document().scheduleStyleRecalc();
        return;
    }

    if (m_styleRecalcTimer.isActive())
        return;

    ASSERT(needsStyleRecalc() || childNeedsStyleRecalc() || childNeedsDistributionRecalc());

    m_styleRecalcTimer.startOneShot(0);

    InspectorInstrumentation::didScheduleStyleRecalculation(this);
}
